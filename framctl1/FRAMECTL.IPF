:userdoc.
:title.A Picture Perfect Control
:h1 res=300.Frame Extensions Sample
:p.
This sample demonstrates how to implement frame extensions.  The
frame extensions are graphic button controls that are created with the
button styles BS_PUSHBUTTON and BS_BITMAP.
:p.
The first extension, the close button, is positioned between the system
menu, and the title bar.  It is created with the BS_SYSCOMMAND style which
generates a WM_SYSCOMMAND message when the button is clicked.  Clicking
this button closes the sample application.
:p.
The second extension which you clicked to bring up this help, is the help
button.  It is positioned between the title bar and the min/max buttons.
It is created with the BS_HELP style which generates a WM_HELP message
when the button is clicked.
:p.
If neither the BS_SYSCOMMAND nor the BS_HELP style is included, a
WM_COMMAND message is generated.
:euserdoc.
