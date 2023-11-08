import bcrypt
import uuid
from pydantic import BaseModel
from error import error_handle
from models import UserTableItem, ShadowUser, UserState
from iot_shadow import get_shadow_state, update_shadow_state
from dynamo import insert_table_item
from password import encrypt_password

class NewUserRequestBody(BaseModel):
    name: str
    username: str
    password: str
    is_sudo: bool = False


@error_handle(
    success_handle=lambda x: {"message": "User successfully created", "uuid": x},
    error_message="Could not insert user",
)
def new_user(event, context):
    # Extracting body from the APIGateway event
    body = NewUserRequestBody.model_validate_json(event.get("body", "{}"))

    body.password = encrypt_password(body.password)

    new_uuid = str(uuid.uuid4())

    insert_table_item(UserTableItem(uuid=new_uuid, **body.model_dump()))

    # Distributed critical section -- may need to think this out more later (would be ridicuosly unlucky for timing to break things)
    shadow_state = get_shadow_state()
    shadow_state.users.append(ShadowUser(uuid=new_uuid, state=UserState.DO_NOT_UNLOCK))
    update_shadow_state(shadow_state)

    return new_uuid