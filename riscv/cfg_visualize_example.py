from cfg import*
from visualize_cfg import*
import sys

args=sys.argv[1:]
insFile,varsFile,savepath=args
c=make_cfg(insFile,varsFile)
visualize(c,savepath,True)
