import cli
from decwar import Decwar
args, kwargs = cli.main()

def main():
    mon = nomad()
    # ha = hawk()
    return
    
def hawk():
    args2 = (*args,)
    kwargs2 = kwargs.copy()
    kwargs2['name'] = 'hawkster'
    kwargs2['team'] = 'emp'
    kwargs2['ship'] = 'h'
    dw = Decwar(*args2, **kwargs2)
    sys = dw.tops10.sys()
    dw.run()
    return dw
def nomad():
    args2 = (*args, 'nomad')
    kwargs2 = kwargs.copy()
    dw = Decwar(*args2, **kwargs2)
    sys = dw.tops10.sys()
    dw.run()
    return dw

if __name__ == "__main__":
    main()
    