from cfg import*
import sys

args=sys.argv[1:]
insname,varsname,savepath=args
c=make_cfg(insname,varsname)
visualize(c,savepath,True)