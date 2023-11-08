from pydantic import BaseModel
from error import error_handle
from models import UserState, ShadowUser
from iot_shadow import get_shadow_state, update_shadow_state

class UserStateUpdateRequestBody(BaseModel):
    state: str


@error_handle(success_handle=lambda x: {"message": "User state updated successfully"})
def update_user_state(event, context):
    body = UserStateUpdateRequestBody.model_validate_json(event.get("body", "{}"))

    newUserState = UserState[body.state]
    uuid = event["pathParameters"]["uuid"]

    # Distributed critical section -- may need to think this out more later (would be ridicuosly unlucky for timing to break things)
    shadow_state = get_shadow_state()
    shadow_state.users = [
        ShadowUser(uuid=uuid, state=newUserState)
        if uuid == shadow_user.uuid
        else shadow_user
        for shadow_user in shadow_state.users
    ]
    update_shadow_state(shadow_state)
