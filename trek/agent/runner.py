import cli
from decwar import Decwar
args, kwargs = cli.main()

def main():
    dw = Decwar(*args, **kwargs).run()
    
def hawk():
    args2 = (*args,)
    kwargs2 = kwargs.copy()
    kwargs2['user'] = 'demo'
    kwargs2['name'] = 'batty'
    kwargs2['team'] = 'emp'
    kwargs2['ship'] = 'h'
    dw = Decwar(*args2, **kwargs2).run()

def nomad():
    args2 = (*args, 'nomad')
    kwargs2 = kwargs.copy()
    kwargs2['user'] = 'decwar'
    kwargs2['name'] = 'nomad'
    kwargs2['team'] = 'fed'
    kwargs2['ship'] = 'y'
    dw = Decwar(*args2, **kwargs2).run()

if __name__ == "__main__":
    main()
