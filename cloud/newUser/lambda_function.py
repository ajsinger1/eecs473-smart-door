import json
import boto3
import bcrypt
import uuid
from botocore.exceptions import ClientError
from pydantic import BaseModel, ValidationError


class NewUserRequestBody(BaseModel):
    name: str
    username: str
    password: str


error_response = {
    "statusCode": 500,
    "body": json.dumps({"error": "Could not insert user"}),
}

# Initialize the DynamoDB resource outside of the handler for better performance
dynamodb = boto3.resource("dynamodb")
table = dynamodb.Table("smart-handle-user-table")


def lambda_handler(event, context):
    # Extracting body from the APIGateway event
    try:
        body = NewUserRequestBody.model_validate_json(event.get("body", "{}"))
        print(body)
    except ValidationError as e:
        print(e.errors())
        return error_response

    plaintext_password = body.password.encode("utf-8")  # Convert string to bytes
    salt = bcrypt.gensalt()  # Generate a salt
    body.password = bcrypt.hashpw(plaintext_password, salt).decode("utf-8")

    user = body.model_dump()
    user["user_id"] = str(uuid.uuid4())

    print(f"user: {user}")

    try:
        # Put the item in the DynamoDB table
        table.put_item(Item=user)
    except ClientError as e:
        print(e.response["Error"]["Message"])
        return error_response

    # Return successful response
    return {
        "statusCode": 200,
        "body": json.dumps({"message": "User successfully created", "user": user}),
    }
