from pydantic import BaseModel
from error import error_handle
from models import Mode
from iot_shadow import get_shadow_state, update_shadow_state

class GetModeResponseBody(BaseModel):
    mode: str

@error_handle(success_handle=lambda x: x)
def get_mode(event, context):
    shadow_state = get_shadow_state()
    return GetModeResponseBody(mode=shadow_state.mode.name).model_dump()
