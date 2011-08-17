#ifndef JUSHELF_H
#define JUSHELF_H

#ifdef _DEBUG
#define DEBUG(format, args...) printf("[%s:%d] "format, __FILE__, __LINE__, ##args)
#else
#define DEBUG(args...)
#endif

typedef enum {
	JU_WINDOW_TYPE_NORMAL,
	JU_WINDOW_TYPE_DESKTOP,
	JU_WINDOW_TYPE_DOCK,
	JU_WINDOW_TYPE_TOOLBAR,
	JU_WINDOW_TYPE_MENU,
	JU_WINDOW_TYPE_UTILITY,
	JU_WINDOW_TYPE_SPLASH,
	JU_WINDOW_TYPE_DIALOG
} JuWindowType;

#endif
