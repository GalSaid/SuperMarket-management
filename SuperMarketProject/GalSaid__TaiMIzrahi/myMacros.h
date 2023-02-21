#pragma once

#define CHECK_RETURN_0(pointer)  pointer == NULL ? 0:1
#define CHECK_MSG_RETURN_0(pointer,msg) if(!pointer) {printf("%s\n" , msg); return 0;}
#define FREE_CLOSE_FILE_RETURN_0(pointer,fp) { free(pointer); fclose(fp); return 0;}
#define CLOSE_RETURN_0(fp) {fclose(fp); return 0;}
