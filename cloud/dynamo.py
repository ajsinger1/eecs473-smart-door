"""
Provides DynamoDB table interaction for 'smart-handle-user-table' using boto3.

Includes functions to get, update, and insert table items modeled by 'UserTableItem'.
Handles DynamoDB resource initialization and defines a default projection expression for item retrieval.

Functions:
- get_table_items: Fetches table items with optional custom expression.
- update_table_item: Updates an item identified by UUID.
- insert_table_item: Inserts a new item into the table.

Models:
- UserTableItem: Represents table schema for item operations.

Exceptions are raised for AWS service interaction issues and model validation errors.
"""

from typing import List
import boto3
from models import UserTableItem

DYNAMO = boto3.resource("dynamodb")
TABLE = DYNAMO.Table("smart-handle-user-table")

DEFAULT_GET_EXPRESSION = {
    f"#{chr(ord('a') + i)}": field
    for i, field in enumerate(UserTableItem.model_fields.keys())
}


def get_table_items(get_expression: dict = None) -> List[UserTableItem]:
    """
    Retrieves items from the 'smart-handle-user-table' DynamoDB table.

    Args:
        get_expression (dict, optional): A dictionary mapping expression attribute names
                                         to the actual table attribute names. If not provided,
                                         defaults to a pre-defined dictionary for all the UserTableItem model's fields.

    Returns:
        List[UserTableItem]: A list of UserTableItem instances representing the items fetched from the table.

    Raises:
        ClientError: An error raised by boto3 if the scan operation fails due to issues with AWS services or the request itself.
    """
    if get_expression is None:
        get_expression = DEFAULT_GET_EXPRESSION

    # Define the projection expression for the scan operation
    projection_expression = ", ".join(get_expression.keys())
    expression_attribute_names = get_expression

    # Fetch items from the DynamoDB
    response = TABLE.scan(
        ProjectionExpression=projection_expression,
        ExpressionAttributeNames=expression_attribute_names,
    )
    items = [
        UserTableItem.model_validate(item_json)
        for item_json in response.get("Items", [])
    ]

    # Paginate if the response is too large
    while "LastEvaluatedKey" in response:
        response = TABLE.scan(
            ProjectionExpression=projection_expression,
            ExpressionAttributeNames=expression_attribute_names,
            ExclusiveStartKey=response["LastEvaluatedKey"],
        )

        items.extend(
            [
                UserTableItem.model_validate(item_json)
                for item_json in response.get("Items", [])
            ]
        )

    return items


def update_table_item(item: UserTableItem) -> None:
    """
    Updates an existing item in the 'smart-handle-user-table' DynamoDB table using the provided UserTableItem instance.

    Args:
        item (UserTableItem): An instance of UserTableItem containing the update data.

    Returns:
        None: This function returns nothing. The item is updated in the DynamoDB table.

    Raises:
        ClientError: An error raised by boto3 if the update_item operation fails due to issues with AWS services or the request itself.
    """
    # Construct expression attribute names and values for the update
    expression_attribute_names = {
        f"#{chr(ord('a') + i)}": field
        for i, (field, value) in enumerate(item.model_dump().items())
        if value is not None and field != "uuid"
    }

    expression_attribute_values = {
        f":{chr(ord('a') + i)}": value
        for i, (field, value) in enumerate(item.model_dump().items())
        if value is not None and field != "uuid"
    }

    # Create the update expression string
    update_expression = "SET" + ", ".join(
        [
            f"{name_key} = {value_key}"
            for name_key, value_key in zip(
                expression_attribute_names.keys(), expression_attribute_values.keys()
            )
        ]
    )

    TABLE.update_item(
        Key={"uuid": item.uuid},
        UpdateExpression=update_expression,
        ExpressionAttributeNames=expression_attribute_names,
        ExpressionAttributeValues=expression_attribute_values,
        ReturnValues="UPDATED_NEW",
    )


def insert_table_item(item: UserTableItem) -> None:
    """
    Inserts a new item into the 'smart-handle-user-table' DynamoDB table.

    Args:
        item (UserTableItem): An instance of UserTableItem representing the new data to be inserted into the table.

    Returns:
        None: This function does not return any value.

    Raises:
        ClientError: If the put_item operation fails due to issues with AWS services.
    """
    TABLE.put_item(Item=item.model_dump())
