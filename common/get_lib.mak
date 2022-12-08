.silent

!ifndef LIB_NAME
!error LIB_NAME is mandatory
!endif

!ifndef ROOT_DIR
ROOT_DIR=..
!endif

!if $(__MAKE__) == 0x360
BC31MODE=
!endif

LIB_FILE=$(LIB_NAME).LIB
LIB_ORG_DIR=$(ROOT_DIR)\rtl\$(LIB_NAME)

!ifdef BC31MODE
LIB_ORG_FILE=$(LIB_ORG_DIR)\OUTWARD\$(LIB_FILE)
LIB_TARGET_FILE=OUTWARD\$(LIB_FILE)
!endif

RETURN_BATCH_FILE=return.bat
RETURN_BATCH=$(LIB_ORG_DIR)\$(RETURN_BATCH_FILE)

!ifdef BC31MODE
# this is just to fix prj2mak utility bug
$(LIB_TARGET_FILE): $(LIB_ORG_FILE)
  copy $** $@

$(LIB_ORG_FILE): execute
!endif

execute:
  copy $(ROOT_DIR)\common\_cd.bat $(RETURN_BATCH) >nul
  cd>>$(RETURN_BATCH)
#
  cd $(LIB_ORG_DIR)
  $(MAKE) publish
#
  $(RETURN_BATCH_FILE)
  del $(RETURN_BATCH)

headers:
  copy $(ROOT_DIR)\common\_cd.bat $(RETURN_BATCH) >nul
  cd>>$(RETURN_BATCH)
#
  cd $(LIB_ORG_DIR)
  $(MAKE) publish_headers
#
  $(RETURN_BATCH_FILE)
  del $(RETURN_BATCH)
