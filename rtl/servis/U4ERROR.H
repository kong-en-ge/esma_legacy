
/*  u4error.h  (c)Copyright Sequiter Software Inc., 1987, 1988, 1989.  All rights reserved.  */

/* 100  General Disk Access Errors */
#define    E_CREATE       100
#define    E_OPEN         120
#define    E_READ         140
#define    E_WRITE        160
#define    E_CLOSE        180

/* 200  Database Specific Errors */
#define    E_BAD_DBF      200
#define    E_D_MISSING    240
#define    E_REC_LENGTH   260
#define    E_CHANGE_SIZE  280

/* 300  Index File Specific Errors */
#define    E_INDEX        300
#define    E_I_CLOSE      310
#define    E_BAD_NDX      320
#define    E_I_MISSING    330
#define    E_I_RECORD     335
#define    E_UNIQUE       340
#define    E_I_TYPE       350
#define    E_I_CHANGED    360
#define    E_KEY_LEN      370
#define    E_NO_INDEX     380

/* 400  Multi-User Errors */
#define    E_LOCK         400
#define    E_UNLOCK       450

/* 500  Expression Errors */
#define    E_BASE_NAME    500
#define    E_COMPILE_NULL 510
#define    E_DATE         515
#define    E_EXPECT       520
#define    E_COMPLETE     530
#define    E_OVERFLOW     540
#define    E_TYPE         550
#define    E_RIGHT        560
#define    E_FUNCTION     570
#define    E_OPERATOR     575
#define    E_VALUE        580
#define    E_STRING_LONG  590

/* 600  Memo File Errors */
#define    E_EDITOR       600
#define    E_MEMO_NAME    620
#define    E_MEMO_SIZE    640

/* 700  Windowing and Menuing Errors */
#define    E_WINDOW_REF   700

/* 800  Extended Routine Errors */
#define    E_RELATED      800
#define    E_CONTROL      820
#define    E_RELATING     840

/* 900  Other Errors */
#define    E_MEMORY       900
#define    E_ALLOCATE     920
#define    E_INTERNAL     950

