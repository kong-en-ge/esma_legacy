.silent

!ifndef TOOL_DIR
!error TOOL_DIR is mandatory
!endif

!ifndef TOOL_NAME
!error TOOL_NAME is mandatory
!endif

TOOLS_DIR=$(ROOT_DIR)\tools

RETURN_BATCH_FILE=return.bat
RETURN_BATCH=$(TOOL_DIR)\$(RETURN_BATCH_FILE)

default: execute

$(TOOLS_DIR):
  IF NOT EXIST $@ MD $@
  
execute: $(TOOLS_DIR)
  copy $(ROOT_DIR)\common\_cd.bat $(RETURN_BATCH) >nul
  cd>>$(RETURN_BATCH)
#
  cd $(TOOL_DIR)
  $(MAKE) release
#
  $(RETURN_BATCH_FILE)
  del $(RETURN_BATCH)
#
  copy $(TOOL_DIR)\outward\$(TOOL_NAME).exe $(TOOLS_DIR)
