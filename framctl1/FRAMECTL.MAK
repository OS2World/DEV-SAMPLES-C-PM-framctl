CC=Icc
# C_SW=-G5 -O+ -W3 -C
C_SW=-G5 -Ti+ -W3 -C

FrameCtl.Exe: FrameCtl.Obj FrameCtl.Res FrameCtl.Hlp
  ilink /DE @FrameCtl.Lnk;
  rc FrameCtl.Res FrameCtl.Exe

FrameCtl.Obj: FrameCtl.C
 $(CC) $(C_SW) -FoFrameCtl.Obj FrameCtl.C

FrameCtl.Res: FrameCtl.Rc FrameCtl.H
 rc -r FrameCtl.Rc

FrameCtl.Hlp: FrameCtl.Ipf
 ipfc FrameCtl.Ipf
