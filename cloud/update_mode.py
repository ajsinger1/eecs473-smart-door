from pydantic import BaseModel
from error import error_handle
from models import Mode
from iot_shadow import get_shadow_state, update_shadow_state

class ModeUpdateRequestBody(BaseModel):
    mode: str


@error_handle(success_handle=lambda x: {"message": "Mode updated successfully"})
def update_mode(event, context):
    body = ModeUpdateRequestBody.model_validate_json(event.get("body", "{}"))

    newMode = Mode[body.mode]

    # Distributed critical section -- may need to think this out more later (would be ridicuosly unlucky for timing to break things)
    shadow_state = get_shadow_state()
    shadow_state.mode = newMode
    update_shadow_state(shadow_state)
