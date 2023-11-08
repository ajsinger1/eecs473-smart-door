import bcrypt
import uuid
from pydantic import BaseModel
from error import error_handle
from models import UserTableItem, ShadowUser, UserState
from iot_shadow import get_shadow_state, update_shadow_state
from dynamo import get_table_items


class GetUserResponseItem(BaseModel):
    uuid: str
    name: str
    username: str
    is_sudo: bool
    state: str


def encrypt_password(password_plain):
    password_plain_bytes = password_plain.encode("utf-8")
    salt = bcrypt.gensalt()  # Generate a salt
    return bcrypt.hashpw(password_plain_bytes, salt).decode("utf-8")


@error_handle(
    success_handle=lambda x: x,
)
def get_users(event, context):
    response = []
    user_table_items = get_table_items()
    print(f"USER TABLE ITEMS: {user_table_items}")
    shadow_state = get_shadow_state()
    print(f"SHADOW_STATE: {shadow_state}")
    for shadow_user in shadow_state.users:
        for user in user_table_items:
            if user.uuid == shadow_user.uuid:
                response.append(
                    GetUserResponseItem(
                        state=shadow_user.state.name, **user.model_dump()
                    ).model_dump()
                )
    return response
