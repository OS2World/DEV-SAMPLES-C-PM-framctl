/************************************************************************/
/*                                                                      */
/* Program name:   FrameCtl.C                                           */
/* Title:          A Picture Perfect Control                            */
/*                 OS/2 Magazine - GUI Corner                           */
/*                 December 1996 issue                                  */
/*                                                                      */
/* Author:         Mark Benge   IBM Corp.                               */
/*                 Matt Smith   Prominare Inc.                          */
/*                                                                      */
/* Description:    Illustrates how to add frame control extensions.     */
/*                                                                      */
/* DISCLAIMER OF WARRANTIES:                                            */
/* -------------------------                                            */
/* The following [enclosed] code is sample code created by IBM          */
/* Corporation and Prominare Inc.  This sample code is not part of any  */
/* standard IBM product and is provided to you solely for the purpose   */
/* of assisting you in the development of your applications.  The code  */
/* is provided "AS IS", without warranty of any kind.  Neither IBM nor  */
/* Prominare shall be liable for any damages arising out of your        */
/* use of the sample code, even if they have been advised of the        */
/* possibility of such damages.                                         */
/************************************************************************/

#define INCL_DOS                   /* Include OS/2 DOS Kernal           */
#define INCL_GPI                   /* Include OS/2 PM GPI Interface     */
#define INCL_WIN                   /* Include OS/2 PM Windows Interface */

#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <malloc.h>

#include "framectl.h"

/************************************************************************/
/* This module contains example installable control that can be used    */
/* by any OS/2 2.x and Warp Presentation Manager application.  The      */
/* sample demonstrates the principles of adding frame control           */
/* extensions such that other extensions can be added using this as     */
/* a model.                                                             */
/*                                                                      */
/* Filename : FrameCtl.C                                                */
/* Version  : 1.00                                                      */
/* Created  : 1996-06-07                                                */
/* Revised  : 1996-09-18                                                */
/* Released :                                                           */
/*                                                                      */
/* Routines:  MRESULT EXPENTRY FrameWndProc( HWND hWnd, ULONG msg,      */
/*                                           MPARAM mp1, MPARAM mp2 )   */
/*            MRESULT EXPENTRY AnimateWndProc( HWND hWnd, ULONG msg,    */
/*                                             MPARAM mp1, MPARAM mp2 ) */
/*            LONG CalcMenuBarMinWidth( HWND hWnd )                     */
/*            LONG CalcSeparatorWidth( HWND hWnd )                      */
/*            int main(int argc, char* argv[] )                         */
/*                                                                      */
/* Copyright ¸ International Business Machines Corp., 1991,1996.        */
/* Copyright ¸ 1989-1996  Prominare Inc.  All Rights Reserved.          */
/*                                                                      */
/************************************************************************/

HAB  hAB;                          /* Anchor Block Handle               */
HWND hwndHelpBtn;                  /* Help Button Handle                */
HWND hwndComboBox;                 /* Combo Box Handle                  */
HWND hwndStaticText;               /* Static Text Handle                */
HWND hwndAnimate;                  /* Animated Window Handle            */
PFNWP DefFrameWndProc;             /* Frame Control Subclass Procedure  */

LONG lMinTitleHeight = 0;          /* Height of the title bar           */
LONG lMinMenuHeight = 0;           /* Height of the menu bar            */
POINTL staticTextPos;              /* Static Text position              */
POINTL animatedCatBmpPos;          /* Animated Cat Bitmap position      */

LONG colorArray[] = { CLR_DARKBLUE, CLR_WHITE, CLR_RED,
                      CLR_GREEN, CLR_BLACK };

/************************************************************************/
/* Prototypes                                                           */
/************************************************************************/
int main( int, char* [] );
MRESULT EXPENTRY FrameWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
MRESULT EXPENTRY AnimateWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 );
LONG CalcMenuBarMinWidth( HWND hWnd );
LONG CalcSeparatorWidth( HWND hWnd );

/* --- FrameWndProc ----------------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to process the messages for the frame      */
/*     control window.                                                  */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND   hWnd; = Window Handle                                     */
/*     ULONG  msg;  = PM Message                                        */
/*     MPARAM mp1;  = Message Parameter 1                               */
/*     MPARAM mp2;  = Message Parameter 2                               */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/* -------------------------------------------------------------------- */
MRESULT EXPENTRY FrameWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch ( msg )
  {
    case WM_FORMATFRAME :
    {
      /******************************************************************/
      /* Query the number of standard frame controls                    */
      /******************************************************************/
      ULONG ulStdCtlCount = (ULONG)DefFrameWndProc( hWnd, msg, mp1, mp2 );
      ULONG ulIdx = ulStdCtlCount;

      /******************************************************************/
      /* Access the SWP array that is passed to us                      */
      /******************************************************************/
      ULONG i;
      PSWP swpArr = (PSWP)mp1;

      for (i=0; i < ulStdCtlCount; i++)
      {
        if ( WinQueryWindowUShort( swpArr[i].hwnd, QWS_ID ) == FID_TITLEBAR )
        {
          /**************************************************************/
          /* Initialize the SWPs for our graphic button control.        */
          /* Since the SWP array for the std frame controls is 0-based  */
          /* and occupy indexes 0 thru n-1 (where n is the total        */
          /* count), we use index n for our graphic button control.     */
          /* Please note that the width and height of our button is the */
          /* same as the height of the title bar.                       */
          /**************************************************************/
          swpArr[ulIdx].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST;
          swpArr[ulIdx].cy = swpArr[i].cy;
          swpArr[ulIdx].cx = swpArr[i].cy;
          swpArr[ulIdx].y  = swpArr[i].y;
          swpArr[ulIdx].hwndInsertBehind = HWND_TOP;

          /**************************************************************/
          /* Calculate the x-position for the help graphic button, and  */
          /* set its window handle.                                     */
          /**************************************************************/
          swpArr[ulIdx].x = swpArr[i].x + swpArr[i].cx - swpArr[ulIdx].cx;
          swpArr[ulIdx].hwnd = hwndHelpBtn;

          /**************************************************************/
          /* Adjust width of the title bar to accomodate the help       */
          /* graphic button.                                            */
          /**************************************************************/
          swpArr[i].cx -= swpArr[ulIdx].cx ;

          /**************************************************************/
          /* Save the minimum title bar height that we'll use in        */
          /* WM_QUERYTRACKINFO processing to calculate the minimum      */
          /* tracking height.                                           */
          /**************************************************************/
          lMinTitleHeight = (swpArr[i].cy > lMinTitleHeight) ? swpArr[i].cy
                                                             : lMinTitleHeight;
        }

        if ( WinQueryWindowUShort( swpArr[i].hwnd, QWS_ID ) == FID_MENU )
        {
          /**************************************************************/
          /* Initialize the SWP for our drop down combo box.  Since     */
          /* the SWP array for the std frame controls is 0-based and    */
          /* occupy indexes 0 thru n-1 (where n is the total count),    */
          /* and the graphic button occupies n, we use index n+1 for    */
          /* our drop down combo box.                                   */
          /**************************************************************/
          swpArr[ulIdx+1].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST;
          swpArr[ulIdx+1].cy = COMBOBOX_HEIGHT;
          swpArr[ulIdx+1].cx = COMBOBOX_WIDTH;
          swpArr[ulIdx+1].y  = swpArr[i].y -
                               (COMBOBOX_HEIGHT - swpArr[i].cy - 1);
          swpArr[ulIdx+1].hwndInsertBehind = HWND_TOP;

          /**************************************************************/
          /* Calculate the x-position for the drop down combo box, and  */
          /* set its window handle.                                     */
          /**************************************************************/
          swpArr[ulIdx+1].x = swpArr[i].x + swpArr[i].cx - swpArr[ulIdx+1].cx;
          swpArr[ulIdx+1].hwnd = hwndComboBox;

          /**************************************************************/
          /* Adjust the width of the menu bar to accomodate our drop    */
          /* down combo box.                                            */
          /**************************************************************/
          swpArr[i].cx -= swpArr[ulIdx+1].cx;

          /**************************************************************/
          /* Save the minimum menu bar height that we'll use in         */
          /* WM_QUERYTRACKINFO processing to calculate the minimum      */
          /* tracking height.                                           */
          /**************************************************************/
          lMinMenuHeight = (swpArr[i].cy > lMinMenuHeight) ? swpArr[i].cy
                                                           : lMinMenuHeight;
        }

        if ( WinQueryWindowUShort( swpArr[i].hwnd, QWS_ID ) == FID_CLIENT )
        {
          /**************************************************************/
          /* Initialize the SWP for our static text control.  Since     */
          /* the SWP array for the std frame controls is 0-based and    */
          /* occupy indexes 0 thru n-1 (where n is the total count),    */
          /* and the graphic button and combo box occupy n and n+1, we  */
          /* use index n+2 for our static text control.                 */
          /**************************************************************/
          swpArr[ulIdx+2].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST;
          swpArr[ulIdx+2].cy = STATICTEXT_HEIGHT;
          swpArr[ulIdx+2].cx = swpArr[i].cx - CAT_BMP_WIDTH - SEPARATOR_WIDTH;
          staticTextPos.y = swpArr[ulIdx+2].y  = swpArr[i].y;
          staticTextPos.x = swpArr[ulIdx+2].x = swpArr[i].x;
          swpArr[ulIdx+2].hwndInsertBehind = HWND_TOP;
          swpArr[ulIdx+2].hwnd = hwndStaticText;

          /**************************************************************/
          /* Initialize the SWP for our animated window control.  We    */
          /* use index n+3 for our animate window control.              */
          /**************************************************************/
          swpArr[ulIdx+3].fl = SWP_MOVE | SWP_SIZE | SWP_NOADJUST;
          swpArr[ulIdx+3].cy = CAT_BMP_HEIGHT;
          swpArr[ulIdx+3].cx = CAT_BMP_WIDTH;
          animatedCatBmpPos.y = swpArr[ulIdx+3].y  = swpArr[i].y;
          animatedCatBmpPos.x = swpArr[ulIdx+3].x = staticTextPos.x +
                                swpArr[ulIdx+2].cx + SEPARATOR_WIDTH;
          swpArr[ulIdx+3].hwndInsertBehind = HWND_TOP;
          swpArr[ulIdx+3].hwnd = hwndAnimate;

          /**************************************************************/
          /* Adjust the origin and height of the client to accomodate   */
          /* our static text control.  Remember to leave enough space   */
          /* for the separator bar.                                     */
          /**************************************************************/
          swpArr[i].y += swpArr[ulIdx+2].cy + SEPARATOR_WIDTH;
          swpArr[i].cy -= (swpArr[ulIdx+2].cy + SEPARATOR_WIDTH);
        }
      }

      /******************************************************************/
      /* Increment the number of frame controls to include our graphic  */
      /* button control, drop down combo box, static text, and animated */
      /* window control.                                                */
      /******************************************************************/
      return( (MRESULT)(ulIdx + 4) );
    }

    case WM_QUERYFRAMECTLCOUNT :
      /******************************************************************/
      /* Query the standard frame controls count and increment to       */
      /* include our graphic button control, drop down combo box,       */
      /* static text, and animated window control.                      */
      /******************************************************************/
      return( (MRESULT)((ULONG)DefFrameWndProc( hWnd, msg, mp1, mp2 ) + 4) );

    case WM_QUERYTRACKINFO :
    {
      /******************************************************************/
      /* Query the default tracking information for the standard frame  */
      /* control.                                                       */
      /******************************************************************/
      BOOL rc = (BOOL)DefFrameWndProc( hWnd, msg, mp1, mp2 );
      PTRACKINFO pTrackInfo = (PTRACKINFO)mp2;

      /******************************************************************/
      /* Calculate the minimum width that we require for the menu bar.  */
      /******************************************************************/
      LONG lMinMenuBarWidth = CalcMenuBarMinWidth( hWnd );

      if ( lMinMenuBarWidth == MIT_ERROR )
        return( (MRESULT)FALSE );

      /******************************************************************/
      /* Calculate and set the minimum tracking width and height.  Note */
      /* that we only use the menu bar to calculate the minmum width.   */
      /* You may want to expand this calculation to include the title   */
      /* bar or other factors.                                          */
      /******************************************************************/
      pTrackInfo->ptlMinTrackSize.x = COMBOBOX_WIDTH +
                                      lMinMenuBarWidth +
                                      (WinQuerySysValue( HWND_DESKTOP,
                                                         SV_CXSIZEBORDER ) * 2);
      pTrackInfo->ptlMinTrackSize.y = STATICTEXT_HEIGHT + SEPARATOR_WIDTH +
                                      lMinTitleHeight + lMinMenuHeight +
                                      WinQuerySysValue( HWND_DESKTOP,
                                                        SV_CYHSCROLL ) - 1 +
                                      (WinQuerySysValue( HWND_DESKTOP,
                                                         SV_CYSIZEBORDER ) * 2);
      return( (MRESULT)TRUE );
    }

    case WM_CALCFRAMERECT:
      {
        /******************************************************************/
        /* Calculate the rectl of the client                              */
        /* control.                                                       */
        /******************************************************************/
        BOOL rc = TRUE;
        PRECTL pRectl = (PRECTL)mp1;
        LONG lExtensionHeight = STATICTEXT_HEIGHT + SEPARATOR_WIDTH;

        if ( SHORT1FROMMP(mp2) )
        {
          /****************************************************************/
          /* Calculate the rectl of the client                            */
          /*--------------------------------------------------------------*/
          /* Call default window procedure to subtract child frame        */
          /* controls from the rectangle's height.                        */
          /****************************************************************/
          LONG lClientHeight;
          rc = (BOOL)DefFrameWndProc( hWnd, msg, mp1, mp2 );

          /****************************************************************/
          /* Position the static text frame extension below the client.   */
          /****************************************************************/
          lClientHeight = pRectl->yTop - pRectl->yBottom;
          if ( lExtensionHeight  > lClientHeight  )
          {
            /**************************************************************/
            /* Extension is taller than client, so set client height to 0.*/
            /**************************************************************/
            pRectl->yTop = pRectl->yBottom;
          }
          else
          {
            /**************************************************************/
            /* Set the origin of the client and shrink it based upon the  */
            /* static text control's height.                              */
            /**************************************************************/
            pRectl->yBottom += lExtensionHeight;
            pRectl->yTop -= lExtensionHeight;
          }
        }
        else
        {
          /****************************************************************/
          /* Calculate the rectl of the frame                             */
          /*--------------------------------------------------------------*/
          /* Call default window procedure to subtract child frame        */
          /* controls from the rectangle's height.                        */
          /* Set the origin of the frame and increase it based upon the   */
          /* static text control's height.                                */
          /****************************************************************/
          pRectl->yBottom -= lExtensionHeight;
          pRectl->yTop += lExtensionHeight;
        }
        return( (MRESULT)rc );
      }

    case WM_PAINT:
      {
        /******************************************************************/
        /* Process WM_PAINT to draw the separator bar.                    */
        /******************************************************************/
        USHORT i;
        POINTL start[5],
               end[5];

        LONG color[5] = { SYSCLR_BUTTONDARK, SYSCLR_BUTTONMIDDLE,
                          SYSCLR_BUTTONMIDDLE, SYSCLR_BUTTONMIDDLE,
                          SYSCLR_BUTTONLIGHT };

        /******************************************************************/
        /* Allow default proc to draw all of the frame.                   */
        /******************************************************************/
        BOOL rc = (BOOL)DefFrameWndProc( hWnd, msg, mp1, mp2 );

        /******************************************************************/
        /* Get presentation space handle for drawing.                     */
        /******************************************************************/
        HPS hps = WinGetPS( hWnd );

        /******************************************************************/
        /* Init the POINTL arrays for drawing the horizontal separator    */
        /******************************************************************/
        LONG lSeparatorWidth = CalcSeparatorWidth( hWnd );

        start[0].x = start[1].x = start[2].x = start[3].x =
            start[4].x = staticTextPos.x;
        start[0].y = start[1].y = start[2].y = start[3].y =
            start[4].y = staticTextPos.y + STATICTEXT_HEIGHT;
        start[1].y += 1;
        start[2].y += 2;
        start[3].y += 3;
        start[4].y += 4;
        end[0] = start[0];
        end[0].x += lSeparatorWidth;
        end[1] = start[1];
        end[1].x += lSeparatorWidth;
        end[2] = start[2];
        end[2].x += lSeparatorWidth;
        end[3] = start[3];
        end[3].x += lSeparatorWidth;
        end[4] = start[4];
        end[4].x += lSeparatorWidth;

        /******************************************************************/
        /* Draw the horizontal separator bar.                             */
        /******************************************************************/
        for (i=0; i< SEPARATOR_WIDTH; i++)
        {
          LONG lSysColor = WinQuerySysColor( HWND_DESKTOP, color[i], 0L );
          GpiSetColor( hps, GpiQueryColorIndex( hps, 0, lSysColor ) );
          GpiMove( hps, &start[i] );
          GpiLine( hps, &end[i] );
        }

        /******************************************************************/
        /* Init the POINTL arrays for drawing the vertical separator      */
        /******************************************************************/
        start[0].x = start[1].x = start[2].x = start[3].x =
            start[4].x = animatedCatBmpPos.x;
        start[0].y = start[1].y = start[2].y = start[3].y =
            start[4].y = staticTextPos.y;
        start[0].x -= 5;
        start[1].x -= 4;
        start[2].x -= 3;
        start[3].x -= 2;
        start[4].x -= 1;
        end[0] = start[0];
        end[0].y += STATICTEXT_HEIGHT;
        end[1] = start[1];
        end[1].y += STATICTEXT_HEIGHT;
        end[2] = start[2];
        end[2].y += STATICTEXT_HEIGHT;
        end[3] = start[3];
        end[3].y += STATICTEXT_HEIGHT;
        end[4] = start[4];
        end[4].y += STATICTEXT_HEIGHT;


        /******************************************************************/
        /* Draw the vertical separator bar.                               */
        /******************************************************************/
        for (i=0; i< SEPARATOR_WIDTH; i++)
        {
          LONG lSysColor = WinQuerySysColor( HWND_DESKTOP, color[i], 0L );
          GpiSetColor( hps, GpiQueryColorIndex( hps, 0, lSysColor ) );
          GpiMove( hps, &start[i] );
          GpiLine( hps, &end[i] );
        }

        /******************************************************************/
        /* Release presentation space handle.                             */
        /******************************************************************/
        WinReleasePS( hps );

        return( (MRESULT)TRUE );
      }

    case WM_COMMAND :
      if ( SHORT1FROMMP(mp2) == CMDSRC_MENU )
      {
        switch ( SHORT1FROMMP(mp1) )
        {
          /****************************************************************/
          /* Process the Exit request.                                    */
          /****************************************************************/
          case MI_FILE_EXIT:
          {
            /**************************************************************/
            /* Close the frame extensions sample application.             */
            /**************************************************************/
            WinPostMsg( hWnd, WM_CLOSE, 0, 0 );
            return( (MRESULT)FALSE );
          }

          /****************************************************************/
          /* Start the animated cat bitmap                                */
          /****************************************************************/
          case MI_CAT_START:
          {
            /**************************************************************/
            /* Start the animation timer so it shows 5 frames a second    */
            /**************************************************************/
            WinStartTimer( hAB, hwndAnimate, TIMER_ANIMATE, 200UL );
            return( (MRESULT)FALSE );
          }

          /****************************************************************/
          /* Stop the animated cat bitmap                                 */
          /****************************************************************/
          case MI_CAT_STOP:
          {
            /**************************************************************/
            /* Stop the animation timer                                   */
            /**************************************************************/
            WinStopTimer( hAB, hwndAnimate, TIMER_ANIMATE );
            return( (MRESULT)FALSE );
          }

          default:
            break;
        } /* end switch */
      }
      return( DefFrameWndProc( hWnd, msg, mp1, mp2 ) );

    case WM_CONTROL :
      if ( (SHORT1FROMMP(mp1) == COMBOBOX_ID)  &&
           (SHORT2FROMMP(mp1) == CBN_LBSELECT) )
      {
        /****************************************************************/
        /* Process the list box selection notification.                 */
        /****************************************************************/
        LONG lFgnColor;
        LONG lSelIndex = (LONG)WinSendMsg( (HWND)mp2,
                                           LM_QUERYSELECTION,
                                           MPFROMLONG(LIT_FIRST),
                                           0 );
        if ( lSelIndex == LIT_NONE )
          return( FALSE );

        lFgnColor = colorArray[lSelIndex];
        WinSetPresParam( WinWindowFromID( hWnd, FID_CLIENT ),
                         PP_FOREGROUNDCOLORINDEX,
                         4UL,
                         &lFgnColor );

      }
      return( DefFrameWndProc( hWnd, msg, mp1, mp2 ) );

    case WM_HELP :
      /******************************************************************/
      /* Process a click on the help graphic button.                    */
      /******************************************************************/
      if ( (USHORT)mp1 == HELP_BUTTON_ID )
      {
        /****************************************************************/
        /* Display the help panel for this sample.                      */
        /****************************************************************/
        return( WinSendMsg( WinQueryHelpInstance( hWnd ),
                            HM_DISPLAY_HELP,
                            mp1,
                            HM_RESOURCEID ) );
      }

    case WM_MENUSELECT :
      {
        char buffer[20];
        if ( SHORT1FROMMP(mp1) != 0xFFFF )
        {
          /* Menu item is being selected */
          if ( WinLoadString( WinQueryAnchorBlock( hWnd ),
                              NULLHANDLE,
                              SHORT1FROMMP(mp1),
                              20,
                              buffer ) )
          {
            WinSetWindowText( hwndStaticText, buffer );
          }
          else
            WinSetWindowText( hwndStaticText, NULL );
        }
        else
        {
          /* Menu item is being deselected */
          /* Get owner window.  If the owner is another menu, then reset the
             info area text to the menu text, otherwise set inactive text */
          HWND hwndOwner = WinQueryWindow( (HWND)mp2, QW_OWNER );
          if (hwndOwner)
          {
            WinQueryClassName( hwndOwner, 20, buffer );
            if ( !memcmp( buffer, "#4", 3 ) )
            {
              SHORT sItem = (SHORT)WinSendMsg( hwndOwner, MM_QUERYSELITEMID,
                                               0, 0 );
              if (( sItem == MIT_ERROR) || ( sItem == MIT_NONE ))
                WinSetWindowText( hwndStaticText, NULL );
              else
              {
                WinSendMsg( hWnd, WM_MENUSELECT,
                            MPFROMSHORT(sItem), MPFROMHWND(hwndOwner) );
              }
            }
            else
              WinSetWindowText( hwndStaticText, NULL );
          }
          else
            WinSetWindowText( hwndStaticText, NULL );
        }

        return( (MRESULT)TRUE );
      }

    case WM_MENUEND :
      WinSetWindowText( hwndStaticText, NULL );
      return( (MRESULT)FALSE );

    default:
      return( DefFrameWndProc( hWnd, msg, mp1, mp2 ) );
  }

  return( (MRESULT)FALSE );
}


/* --- CalcMenuBarMinWidth ---------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to dynamically calculate the minimum menu  */
/*     bar width required to display the entire menu bar.               */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND   hWnd; = Frame Window Handle                               */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/* -------------------------------------------------------------------- */
LONG CalcMenuBarMinWidth( HWND hWnd )
{
  RECTL rect;
  LONG i;
  LONG lMinWidth = 0;
  HWND hwndMenuBar = WinWindowFromID( hWnd, FID_MENU );

  /**********************************************************************/
  /* Get the count of the menu items on the menu bar.                   */
  /**********************************************************************/
  LONG lMenuItemsCount = (LONG)WinSendMsg( hwndMenuBar,
                                           MM_QUERYITEMCOUNT,
                                           0,
                                           0 );

  for ( i=0; i < lMenuItemsCount; i++ )
  {
    /********************************************************************/
    /* Get the id of the menu item.                                     */
    /********************************************************************/
    LONG itemID = (LONG)WinSendMsg( hwndMenuBar,
                                     MM_ITEMIDFROMPOSITION,
                                     MPFROMLONG( i ),
                                     0 );
    if ( itemID == MIT_ERROR )
      return( MIT_ERROR );

    /********************************************************************/
    /* Get the rectangle for the menu item.                             */
    /********************************************************************/
    if ( WinSendMsg( hwndMenuBar,
                     MM_QUERYITEMRECT,
                     MPFROM2SHORT((USHORT)itemID, FALSE),
                     MPFROMP(&rect) ) )
    {
      /******************************************************************/
      /* Calculate the item's width and add it to the width of the      */
      /* other items.                                                   */
      /******************************************************************/
      lMinWidth += (rect.xRight - rect.xLeft);
    }
    else
      return( MIT_ERROR );
  }

  return( lMinWidth );
}

/* --- CalcSeparatorWidth ----------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to calculate the width of the separator    */
/*     bar that we draw to visually separate the static text control    */
/*     and animated cat bitmap from the client.  It is the same width   */
/*     as the client, so we return the client's width minus 1.          */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/* -------------------------------------------------------------------- */
LONG CalcSeparatorWidth( HWND hWnd )
{
  RECTL rectl;
  LONG lWidth = 0;

  if ( WinQueryWindowRect( WinWindowFromID( hWnd, FID_CLIENT ), &rectl ) )
    lWidth = rectl.xRight - rectl.xLeft - 1;

  lWidth = (lWidth < 0) ? 0 : lWidth;
  return( lWidth );
}

/* --- AnimateWndProc --------------------------------- [ Private ] --- */
/*                                                                      */
/*     This function is used to process the messages for the animated   */
/*     control window.                                                  */
/*                                                                      */
/*     Upon Entry:                                                      */
/*                                                                      */
/*     HWND   hWnd; = Window Handle                                     */
/*     ULONG  msg;  = PM Message                                        */
/*     MPARAM mp1;  = Message Parameter 1                               */
/*     MPARAM mp2;  = Message Parameter 2                               */
/*                                                                      */
/*     Upon Exit:                                                       */
/*                                                                      */
/* -------------------------------------------------------------------- */
MRESULT EXPENTRY AnimateWndProc( HWND hWnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  PANIMATE pAnimate;       /* Animation Data              */
  HPS  hPS;                /* Presentation Space Handle   */

  switch ( msg )
  {
    case WM_CREATE :
      /******************************************************************/
      /* Allocate memory for the animation data and save the address    */
      /* in the window's reserved memory.                               */
      /******************************************************************/
      pAnimate = (PANIMATE)malloc( sizeof(ANIMATE) );
      WinSetWindowULong( hWnd, QWL_USER, (ULONG)pAnimate );

      /******************************************************************/
      /* Load the animation bitmap.  The bitmap image is 280 x 28 which */
      /* yields 10-28 x 28 frames.                                      */
      /******************************************************************/
      pAnimate->hbm = GpiLoadBitmap( hPS = WinGetPS(hWnd),
                                     (HMODULE)NULL,
                                     CATS_BMP_ID, 0L, 0L );

      /******************************************************************/
      /* Set the starting image index to start at the first frame       */
      /******************************************************************/
      pAnimate->iImage = 0UL;

      /******************************************************************/
      /* Initialize the source and target bitmap points.  The target    */
      /* points will always be the same whereas the source points will  */
      /* slide across the bitmap image for each frame shown.            */
      /******************************************************************/
      pAnimate->aptl[0].x = 0L;
      pAnimate->aptl[0].y = 0L;
      pAnimate->aptl[1].x = 28L;
      pAnimate->aptl[1].y = 28L;
      pAnimate->aptl[2].x = 0L;
      pAnimate->aptl[2].y = 0L;
      pAnimate->aptl[3].x = 28L;
      pAnimate->aptl[3].y = 28L;
      WinReleasePS( hPS );
      break;

    case WM_TIMER :
      switch ( SHORT1FROMMP(mp1) )
      {
        /****************************************************************/
        /* 1/5 second has elapsed, show a new image                     */
        /****************************************************************/
        case TIMER_ANIMATE :
          {
            /************************************************************/
            /* Get the animation data from the window's reserved memory */
            /************************************************************/
            pAnimate = (PANIMATE)WinQueryWindowULong( hWnd, QWL_USER );

            /************************************************************/
            /* Check to see if the image has wrapped around to the      */
            /* beginning again and if so, set the the source points     */
            /* back to the start of the bitmap image otherwise slide    */
            /* the source points across by 28 bits.                     */
            /************************************************************/
            if ( (pAnimate->iImage++ % 10UL) == 0 )
            {
              pAnimate->aptl[2].x = 0L;
              pAnimate->aptl[3].x = 28L;
            }
            else
            {
              pAnimate->aptl[2].x += 28L;
              pAnimate->aptl[3].x += 28L;
            }

            /************************************************************/
            /* Display the new bitmap image                             */
            /************************************************************/
            GpiWCBitBlt( hPS = WinGetPS(hWnd),
                         pAnimate->hbm, 4L,
                         pAnimate->aptl, ROP_SRCCOPY, BBO_IGNORE );
            WinReleasePS( hPS );
          }
          break;

        default :
          return( WinDefWindowProc( hWnd, msg, mp1, mp2 ) );
      } /* end switch */
      break;

    case WM_PAINT :
      {
        /****************************************************************/
        /* Get the animation data from the window's reserved memory     */
        /****************************************************************/
        pAnimate = (PANIMATE)WinQueryWindowULong( hWnd, QWL_USER );

        /****************************************************************/
        /* Display the current bitmap image                             */
        /****************************************************************/
        GpiWCBitBlt( hPS = WinBeginPaint( hWnd, (HWND)NULL, NULL ),
                     pAnimate->hbm, 4L,
                     pAnimate->aptl, ROP_SRCCOPY, BBO_IGNORE );
        WinEndPaint( hPS );
        break;
      }

    case WM_DESTROY :
      /******************************************************************/
      /* Get the animation data from the window's reserved memory       */
      /******************************************************************/
      pAnimate = (PANIMATE)WinQueryWindowULong( hWnd, QWL_USER );

      /******************************************************************/
      /* Delete the bitmap                                              */
      /******************************************************************/
      GpiDeleteBitmap( pAnimate->hbm );

      /******************************************************************/
      /* Release the memory used for the animation data                 */
      /******************************************************************/
      free( pAnimate );
      break;

    default :
      return( WinDefWindowProc( hWnd, msg, mp1, mp2 ) );
  } /* end switch */

  return( (MRESULT)FALSE );
}


/* --- main ----------------------------------------------------------- */
/*     Main function.                                                   */
/* -------------------------------------------------------------------- */
int main( int argc, char* argv[] )
{
  HMQ      hMQ;                    /* Message Queue Handle              */
  HWND     hwndClient;             /* Client Handle                     */
  HWND     hwndFrame;              /* Frame Handle                      */
  QMSG     qmsg;                   /* PM Message Queue Holder           */
  ULONG    flCreateFlags;          /* Window Creation Flags             */
  LONG     lFgnColor;              /* Foreground Color Holder           */
  LONG     lBgnColor;              /* Background Color Holder           */
  HWND     hwndHelp;               /* Help Instance Handle              */
  HELPINIT helpInit;               /* Help Instance Structure           */
  LBOXINFO lboxinfo;               /* List Box Info Structure           */
  BOOL     bWarpV4 = FALSE;        /* OS/2 Warp V4.0 indicator          */
  ULONG    ulVersion;              /* Version Return Variable           */
  LONG     lCount,
           lWidth,
           lHeight;
  ULONG    i;
  PSZ      itemsArray[] = { "Blue Text",  "White Text", "Red Text",
                            "Green Text", "Black Text" };

  /**********************************************************************/
  /* Initialize the program for PM, create the message queue, and set   */
  /* the codepage.                                                      */
  /**********************************************************************/
  hAB = WinInitialize( 0 );
  hMQ = WinCreateMsgQueue( hAB, 0 );
  WinSetCp( hMQ, 850 );

  /**********************************************************************/
  /* Create a standard frame window, specifying a static text control   */
  /* for the client area.                                               */
  /**********************************************************************/
  flCreateFlags = FCF_TITLEBAR | FCF_SYSMENU | FCF_MENU | FCF_SIZEBORDER |
                  FCF_MINMAX | FCF_VERTSCROLL | FCF_HORZSCROLL | FCF_TASKLIST;

  hwndFrame = WinCreateStdWindow( HWND_DESKTOP,
                                  0,
                                  &flCreateFlags,
                                  WC_STATIC,
                                  "Frame Extensions",
                                  (WS_VISIBLE | SS_TEXT | DT_CENTER |
                                   DT_VCENTER),
                                  (HMODULE)0L,
                                  FRAME_WND_ID,
                                  &hwndClient );
  if ( !hwndFrame )
    return( TRUE );

  /**********************************************************************/
  /* Set the text for the static text control as well as the            */
  /* foreground and background color of the control.                    */
  /**********************************************************************/
  lFgnColor = colorArray[0];
  WinSetPresParam( hwndClient, PP_FOREGROUNDCOLORINDEX, 4UL, &lFgnColor );
  lBgnColor = SYSCLR_DIALOGBACKGROUND;
  WinSetPresParam( hwndClient, PP_BACKGROUNDCOLORINDEX, 4UL, &lBgnColor );

  WinSetWindowText( hwndClient, "Frame Extensions Test" );

  /**********************************************************************/
  /* Create and associate the help instance.                            */
  /**********************************************************************/
  helpInit.cb                       = sizeof(HELPINIT);
  helpInit.ulReturnCode             = 0;
  helpInit.pszTutorialName          = 0;
  helpInit.phtHelpTable             = (PHELPTABLE)MAKELONG(0, 0xffff);
  helpInit.hmodHelpTableModule      = 0;
  helpInit.hmodAccelActionBarModule = 0;
  helpInit.idAccelTable             = 0;
  helpInit.idActionBar              = 0;
  helpInit.pszHelpWindowTitle       = "Frame Extensions Sample";
  helpInit.fShowPanelId             = CMIC_HIDE_PANEL_ID;
  helpInit.pszHelpLibraryName       = "framectl.hlp";

  hwndHelp = WinCreateHelpInstance( hAB, &helpInit );
  if ( !hwndHelp )
    return( TRUE );

  if ( !WinAssociateHelpInstance( hwndHelp, hwndFrame ) )
    return( TRUE );

  /**********************************************************************/
  /* Use the "new look" help bitmap for OS/2 Warp V4 (aka Merlin) if    */
  /* V4 is detected.                                                    */
  /**********************************************************************/
  ulVersion = WinQueryVersion( hAB );
  if ( ((ulVersion & 0xFF) == 20) &&
       (((ulVersion >> 8) & 0xFF) == 40) )
  {
    bWarpV4 = TRUE;
  }

  /**********************************************************************/
  /* Create the help graphic button which we will use as a frame        */
  /* extension on the title bar.                                        */
  /**********************************************************************/
  hwndHelpBtn = WinCreateWindow( hwndFrame,
                                 WC_BUTTON,
                                 ( (bWarpV4) ? "#400" : "#300" ),
                                 (BS_BITMAP | BS_PUSHBUTTON | BS_NOBORDER |
                                  BS_NOPOINTERFOCUS | BS_AUTOSIZE |
                                  BS_HELP | WS_VISIBLE),
                                 0L, 0L, -1L, -1L,
                                 hwndFrame,
                                 HWND_TOP,
                                 HELP_BUTTON_ID,
                                 (PVOID)NULL,
                                 (PVOID)NULL );
  if ( !hwndHelpBtn )
    return( TRUE );

  /**********************************************************************/
  /* Create the drop down combo box which we will use as a frame        */
  /* extension on the menu bar.                                         */
  /**********************************************************************/
  hwndComboBox = WinCreateWindow( hwndFrame,
                                 WC_COMBOBOX,
                                 (PSZ)NULL,
                                 (CBS_DROPDOWNLIST |  WS_VISIBLE),
                                 0L, 0L, -1L, -1L,
                                 hwndFrame,
                                 HWND_TOP,
                                 COMBOBOX_ID,
                                 (PVOID)NULL,
                                 (PVOID)NULL );
  if ( !hwndComboBox )
    return( TRUE );

#if 0
  lboxinfo.lItemIndex = LIT_END;
  lboxinfo.ulItemCount = 5;

  lCount = (LONG)WinSendMsg( hwndComboBox, LM_INSERTMULTITEMS,
                             &lboxinfo, itemsArray[0] );
#endif

  /**********************************************************************/
  /* Populate the drop down combo box.                                  */
  /**********************************************************************/
  for (i = 0; i < 5; i++)
  {
    WinSendMsg( hwndComboBox, LM_INSERTITEM, (MPARAM)LIT_END, itemsArray[i] );
  }

  WinSendMsg( hwndComboBox, LM_SELECTITEM, 0L, (MPARAM)TRUE );

  /**********************************************************************/
  /* Create the static text control which we will set as a frame        */
  /* extension below the client.                                        */
  /**********************************************************************/
  hwndStaticText = WinCreateWindow( hwndFrame,
                                    WC_STATIC,
                                    (PSZ)NULL,
                                    (SS_TEXT | DT_LEFT | DT_VCENTER |
                                     DT_ERASERECT | WS_VISIBLE ),
                                    0L, 0L, -1L, -1L,
                                    hwndFrame,
                                    HWND_TOP,
                                    STATICTEXT_ID,
                                    (PVOID)NULL,
                                    (PVOID)NULL );
  if ( !hwndStaticText )
    return( TRUE );

  WinSetPresParam( hwndStaticText, PP_BACKGROUNDCOLORINDEX, 4UL, &lBgnColor );

  /**********************************************************************/
  /* Register the animated control window's class.                      */
  /**********************************************************************/
  if ( !WinRegisterClass(hAB, "Animate.Window", (PFNWP)AnimateWndProc,
             CS_SYNCPAINT | CS_MOVENOTIFY | CS_SIZEREDRAW, 4UL) )
     return(0);

  /**********************************************************************/
  /* Create the animated window control which we will set as a frame    */
  /* extension below the client.                                        */
  /**********************************************************************/
  hwndAnimate = WinCreateWindow( hwndFrame,
                                 "Animate.Window",
                                 (PSZ)NULL,
                                 WS_VISIBLE,
                                 0L, 0L, -1L, -1L,
                                 hwndFrame,
                                 HWND_TOP,
                                 ANIMATED_ID,
                                 (PVOID)NULL,
                                 (PVOID)NULL );

  /**********************************************************************/
  /* Subclass the frame control.  The subclass procedure is where we    */
  /* will add the frame extensions during processing of WM_FORMATFRAME. */
  /**********************************************************************/
  DefFrameWndProc = WinSubclassWindow( hwndFrame, (PFNWP)FrameWndProc );

  if ( !DefFrameWndProc )
    return( TRUE );

  /**********************************************************************/
  /* Indicate to PM that the frame needs updating.                      */
  /**********************************************************************/
  WinSendMsg( hwndFrame, WM_UPDATEFRAME, (MPARAM)~0, NULL );

  /**********************************************************************/
  /* Position (center on the desktop), size, and show the frame window. */
  /**********************************************************************/
  lWidth = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );
  lHeight = WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN );
  WinSetWindowPos( hwndFrame,
                   HWND_TOP,
                   (lWidth - FRAME_WIDTH)/ 2,
                   (lHeight - FRAME_HEIGHT)/ 2,
                   FRAME_WIDTH,
                   FRAME_HEIGHT,
                   SWP_MOVE | SWP_SIZE | SWP_SHOW | SWP_ACTIVATE );

  /**********************************************************************/
  /* Message dispatch loop.                                             */
  /**********************************************************************/
  while ( WinGetMsg( hAB, &qmsg, (HWND)NULL, 0, 0 ) )
    WinDispatchMsg( hAB, &qmsg );

  /**********************************************************************/
  /* Termination processing                                             */
  /**********************************************************************/
  WinDestroyHelpInstance( hwndHelp );
  WinDestroyWindow( hwndFrame );
  WinDestroyMsgQueue( hMQ );
  WinTerminate( hAB );
  return( FALSE );
}

