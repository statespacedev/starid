import argparse

def main():
    cli = argparse.ArgumentParser()
    cli.add_argument('-i', '--ip', default='192.168.178.21', type=str)
    cli.add_argument('-p', '--port', default=2030, type=int)
    cli.add_argument('-u', '--ppn', default='demo', type=str)
    cli.add_argument('-n', '--name', default='demo', type=str)
    # cli.add_argument('-t', '--team', choices=['f', 'e'], default='f', type=str)
    # cli.add_argument('-s', '--ship', choices=['e', 'f', 'i', 'l', 'n', 's', 't', 'v', 'y', 'b', 'c', 'd', 'g', 'h', 'j', 'm', 'p', 'w'], default='y', type=str)
    cli.add_argument('--nomad', dest='nomad', action='store_true', default=False)
    cli2 = cli.parse_args()
    args = set()
    if cli2.nomad: args.add('nomad') 
    args = tuple(args)
    kwargs = {}
    kwargs['ip'] = cli2.ip
    kwargs['port'] = cli2.port
    kwargs['ppn'] = cli2.ppn
    kwargs['name'] = cli2.name
    # kwargs['team'] = cli2.team
    # kwargs['ship'] = cli2.ship
    if 'nomad' in args:
        kwargs['ppn'] = 'decwar'
        kwargs['name'] = 'nomad'
    return args, kwargs
    
if __name__ == "__main__":
    main()
    