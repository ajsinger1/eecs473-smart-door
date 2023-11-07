import json
import boto3
import bcrypt
import uuid
from pydantic import BaseModel, ValidationError
from typing import Optional


class UserUpdateRequestBody(BaseModel):
    name: Optional[str] = None
    username: Optional[str] = None
    password: Optional[str] = None
    is_sudo: Optional[bool] = None


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

    # Extract path parameter
    user_id = event["pathParameters"]["user_id"]

    # Prepare the update expression based on provided fields
    update_expressions = []
    expression_attribute_names = {}
    expression_attribute_values = {}
    if body.username is not None:
        update_expressions.append("#u = :u")
        expression_attribute_names["#u"] = "username"
        expression_attribute_values[":u"] = body.username
    if body.name is not None:
        update_expressions.append("#n = :n")
        expression_attribute_names["#n"] = "name"
        expression_attribute_values[":n"] = body.name
    if body.password is not None:
        update_expressions.append("#p = :p")
        plaintext_password = body.password.encode("utf-8")  # Convert string to bytes
        salt = bcrypt.gensalt()  # Generate a salt
        body.password = bcrypt.hashpw(plaintext_password, salt).decode("utf-8")
        expression_attribute_names["#p"] = "password"
        expression_attribute_values[":p"] = body.password
    if body.is_sudo is not None:
        update_expressions.append("#s = :s")
        expression_attribute_names["#s"] = "is_sudo"
        expression_attribute_values[":s"] = body.is_sudo

    # Construct the final update expression
    if not update_expressions:
        return {
            "statusCode": 400,
            "body": json.dumps({"error": "No update fields provided"}),
        }

    update_expression = "SET " + ", ".join(update_expressions)

    try:
        # Perform the update operation
        response = table.update_item(
            Key={"user_id": user_id},
            UpdateExpression=update_expression,
            ExpressionAttributeNames=expression_attribute_names,
            ExpressionAttributeValues=expression_attribute_values,
            ReturnValues="UPDATED_NEW",
        )
        # Return a successful response
        return {
            "statusCode": 200,
            "body": json.dumps(
                {
                    "message": "Item updated successfully",
                    "updatedAttributes": response["Attributes"],
                }
            ),
        }

    except boto3.exceptions.Boto3Error as e:
        # Handle DynamoDB errors
        return {
            "statusCode": 500,
            "body": json.dumps({"error": "Internal server error", "message": str(e)}),
        }
