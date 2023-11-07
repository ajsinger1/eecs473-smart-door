import boto3
import json

# Initialize a boto3 DynamoDB client
dynamodb = boto3.resource("dynamodb")


def lambda_handler(event, context):
    table = dynamodb.Table("smart-handle-user-table")

    # Define the projection expression for the scan operation
    projection_expression = "#id, #n, #u, #s, #su"
    expression_attribute_names = {
        "#id": "user_id",
        "#n": "name",
        "#u": "username",
        "#s": "state",
        "#su": "is_sudo",
    }

    try:
        # Fetch items from the DynamoDB table with specific fields: "name" and "email"
        response = table.scan(
            ProjectionExpression=projection_expression,
            ExpressionAttributeNames=expression_attribute_names,
        )
        items = response.get("Items", [])

        # Paginate if the response is too large
        while "LastEvaluatedKey" in response:
            response = table.scan(
                ProjectionExpression=projection_expression,
                ExpressionAttributeNames=expression_attribute_names,
                ExclusiveStartKey=response["LastEvaluatedKey"],
            )
            items.extend(response.get("Items", []))

        # Return the items as a JSON response
        return {
            "statusCode": 200,
            "body": json.dumps(items),
            "headers": {"Content-Type": "application/json"},
        }
    except Exception as e:
        # Return a server error response
        return {
            "statusCode": 500,
            "body": json.dumps({"error": "Internal server error", "message": str(e)}),
            "headers": {"Content-Type": "application/json"},
        }
