from pydantic import BaseModel
from error import error_handle
from models import UserTableItem
from password import encrypt_password
from dynamo import update_table_item
import json


@error_handle(success_handle=lambda x: {"message": "User info updated successfully"})
def update_user_info(event, context):
    # Extract path parameter
    uuid = event["pathParameters"]["uuid"]
    updated_user = UserTableItem(uuid=uuid, **json.loads(event.get("body", "{}")))
    if updated_user.password is not None:
        updated_user.password = encrypt_password(updated_user.password)
    update_table_item(updated_user)
