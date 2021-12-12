import graphviz

def visualize(cfg,savepath,save=False):
    dot=gv.Digraph(name=savepath,
                   graph_attr={'bgcolor':'#24283B','pad':'0.5'},
                  node_attr={'fontsize':'12','fontcolor':'#ffffff','color':'#E0AF68','fontname':'Hack, monospace','shape':'underline'},
                  edge_attr={'color':'#E0AF68','arrowsize':'0.5','fontcolor':'#ffffff','fontsize':'12'})
    gen=lambda i:'{}' if cfg[i]['gen']==set() else str(cfg[i]['gen'])
    kill=lambda i:'{}' if cfg[i]['kill']==set() else str(cfg[i]['kill'])
    name=lambda i: '---'+i+'---\nGen '+gen(i)+'\nKill '+kill(i)
    for l in cfg:
        for n in cfg[l]['succ']: dot.node(name(n))
    for l in cfg:
        for node_name,edge_name in zip(cfg[l]['succ'],cfg[l]['succ_labels']):  
            dot.edge(name(l),name(node_name),label=edge_name)
    if save: dot.render(savepath)
    return dot