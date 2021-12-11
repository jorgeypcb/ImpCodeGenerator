def iterate_liveness(init_data=None,**kwargs):
    cfg=kwargs['cfg']
    data=copy.deepcopy(init_data)
    nBlocks=str(max([int(i) for i in cfg]))    
    
    if init_data is None: return {'in':{i:set() for i in list(cfg)},'out':{i:set() for i in list(cfg)}}
    
    def get_in(blocklabel):
        _in=cfg[blocklabel]['gen'].union(data['out'][blocklabel]-cfg[blocklabel]['kill'])
        return _in
    
    def get_out(blocklabel):
        _out=set()
        for j in cfg[blocklabel]['succ']: _out=_out.union(data['in'][j])
        return _out
    
    assert(not len(get_out(nBlocks))) #out set for the last block should be empty
    blocklabel=int(nBlocks)
    while 0<blocklabel:
        data['out'][str(blocklabel)]=get_out(str(blocklabel))
        data['in'][str(blocklabel)]=get_in(str(blocklabel))
        blocklabel-=1
    return data