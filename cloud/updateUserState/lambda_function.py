import json
import boto3
import uuid
from pydantic import BaseModel, ValidationError
from typing import Optional
from enum import Enum


class UserStateEnum(str, Enum):
    DO_NOT_UNLOCK = "DO_NOT_UNLOCK"
    UNLOCK = "UNLOCK"
    LOCKOUT = "LOCKOUT"


class UserUpdateRequestBody(BaseModel):
    state: UserStateEnum


# Initialize the DynamoDB resource outside of the handler for better performance
dynamodb = boto3.resource("dynamodb")
table = dynamodb.Table("smart-handle-user-table")


def lambda_handler(event, context):
    # Extracting body from the APIGateway event
    try:
        body = UserUpdateRequestBody.model_validate_json(event.get("body", "{}"))
        print(body)
    except ValidationError as e:
        print(e.errors())
        return {
            "statusCode": 500,
            "body": json.dumps({"error": "Internal server error", "message": str(e)}),
        }

    user_id = event["pathParameters"]["user_id"]

    # TODO: VALIDATE USER_ID IN DB

    # TODO: VALIDATE SUDO PERMISSION

    update_expression = "SET #s = :s"
    expression_attribute_names = {"#s": "state"}
    expression_attribute_values = {":s": body.state}

    try:
        # Perform the update operation
        response = table.update_item(
            Key={"user_id": user_id},
            UpdateExpression=update_expression,
            ExpressionAttributeNames=expression_attribute_names,
            ExpressionAttributeValues=expression_attribute_values,
            ReturnValues="UPDATED_NEW",
        )

        # TODO: SEND UPDATE TO IOT TOPIC

        # Return a successful response
        return {
            "statusCode": 200,
            "body": json.dumps({"message": "State updated successfully"}),
        }

    except boto3.exceptions.Boto3Error as e:
        # Handle DynamoDB errors
        return {
            "statusCode": 500,
            "body": json.dumps({"error": "Internal server error", "message": str(e)}),
        }
