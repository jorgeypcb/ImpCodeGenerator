def iterate_reaching_definitions(init_data=None,**kwargs):
    cfg=kwargs['cfg']
    data=copy.deepcopy(init_data)
    allvars=kwargs['allvars']
    labels=list(cfg)
    
    #Empty data structure, for first iteration in fixed point analysis
    if init_data is None: return {'rd_entry':{i:set() for i in labels},'rd_exit':{i:set() for i in labels}}
    
    def entry(i):
        _entry=copy.deepcopy(init_data['rd_entry'][i])
        if i=='1':
            return set([(v,'?') for v in allvars])
        else:
            for l in labels:
                if i in cfg[l]['succ']:
                    _entry=_entry.union(init_data['rd_exit'][l])
        return _entry
    
    def exit(i):
        _exit=copy.deepcopy(init_data['rd_entry'][i])
        for v in cfg[i]['kill']:
            for l in labels+['?']:
                _exit=_exit-set([(v,l)])
            _exit.add((v,i))
        return _exit
    
    for i in labels:
        data['rd_entry'][i]=entry(i)
    for i in labels:
        data['rd_exit'][i]=exit(i)
    return data