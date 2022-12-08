!ifndef MAKE_LIB_DEFINED
!error Include make_lib.inc first
!endif

publish_headers: $(RTL_HEADERS) $(PUBLISHED_HEADERS)

publish: $(PUBLISHED_LIB) publish_headers
