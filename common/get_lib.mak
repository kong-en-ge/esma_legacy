.silent

!ifndef LIB_NAME
!error LIB_NAME is mandatory
!endif

!ifndef RETURN_DIR
!error RETURN DIR is mandatory
!endif

LIB_FILE=$(LIB_NAME).LIB
LIB_ORG_DIR=..\$(LIB_NAME)
LIB_ORG_FILE=$(LIB_ORG_DIR)\OUTWARD\$(LIB_FILE)
LIB_TARGET_FILE=LIB\$(LIB_FILE)

$(LIB_TARGET_FILE): $(LIB_ORG_FILE)
  copy $** $@

$(LIB_ORG_FILE): #..\$(LIB_NAME)
  cd $(LIB_ORG_DIR)
  $(MAKE) project
  cd $(RETURN_DIR)
