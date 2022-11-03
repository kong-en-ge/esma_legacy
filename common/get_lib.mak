.silent

!ifndef LIB_NAME
!error LIB_NAME is mandatory
!endif

LIB_FILE=$(LIB_NAME).LIB
LIB_ORG_DIR=..\$(LIB_NAME)
LIB_ORG_FILE=$(LIB_ORG_DIR)\OUTWARD\$(LIB_FILE)
LIB_TARGET_FILE=LIB\$(LIB_FILE)
RETURN_BATCH_FILE=return.bat
RETURN_BATCH=$(LIB_ORG_DIR)\$(RETURN_BATCH_FILE)

$(LIB_TARGET_FILE): $(LIB_ORG_FILE)
  IF NOT EXIST $(@D)\. md $(@D)\.
  copy $** $@

$(LIB_ORG_FILE):
  copy ..\common\_cd.bat $(RETURN_BATCH) >nul
  cd>>$(RETURN_BATCH)
#
  cd $(LIB_ORG_DIR)
  $(MAKE) project
#
  $(RETURN_BATCH_FILE)
  del $(RETURN_BATCH)