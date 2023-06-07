/*------------------------
   FRAMECTL.H header file
  ------------------------*/

#define FRAME_WND_ID      100
#define CAT_SCRATCH_ID    200
#define HELP_BUTTON_ID    300
#define HELP_BUTTON2_ID   400
#define COMBOBOX_ID       500
#define STATICTEXT_ID     600
#define ANIMATED_ID       700
#define CATS_BMP_ID       800

#define MI_FILE           1000
#define MI_FILE_EXIT      1100
#define MI_CAT            1200
#define MI_CAT_START      1300
#define MI_CAT_STOP       1400

#define FRAME_WIDTH       300L
#define FRAME_HEIGHT      150L

#define COMBOBOX_WIDTH    120L
#define COMBOBOX_HEIGHT   100L

#define STATICTEXT_HEIGHT 29L
#define SEPARATOR_WIDTH    5L

#define CAT_BMP_WIDTH     28L
#define CAT_BMP_HEIGHT    29L

typedef struct _ANIMATE
{
  HBITMAP hbm;
  POINTL  aptl[4];
  ULONG   iImage;
} ANIMATE, *PANIMATE;


#define TIMER_ANIMATE (TID_USERMAX - 1)
