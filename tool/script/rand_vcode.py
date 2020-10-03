#!/usr/bin/python
# -*- coding: utf-8 -*-
"""Generate fake V_CODE keys for use in examples
"""

__version__ = '1.0.0'
__date__    = '2020-10-03'
__author__  = 'Robert Jordan'

__all__ = ['rand_vcode']

#######################################################################################

import random, string

CHARSET = string.digits + string.ascii_uppercase

def rand_vcode(brand:str=None, postfix:bool=False):
    """rand_vcode([None]) -> '{code}'
    rand_vcode(brand, [False]) -> '{brand}-{code}'
    rand_vcode(brand, True) -> '{code}-{brand}'
    
    Returns a generated fake V_CODE with an optional brand (prefix or postfix)
    """
    rng = random.Random()
    code = ''.join(CHARSET[rng.randint(0, len(CHARSET)-1)] for _ in range(8))
    if brand is None:
        return code
    return ('{1}-{0}' if postfix else '{0}-{1}').format(brand, code)


def main(args=None):
    import argparse
    parser = argparse.ArgumentParser(add_help=True,
        description='Generate fake V_CODE keys for use in examples')

    parser.add_argument('brand', metavar='BRAND', default=None,
        nargs='?', help='name attatched to V_CODE "WM-XXXXXXXX"')
    
    prefix_group = parser.add_mutually_exclusive_group(required=False)
    prefix_group.add_argument('-n', '--nobrand',
        dest='nobrand', action='store_true', default=None,
        help='BRAND is not added "XXXXXXXX"')
    prefix_group.add_argument('-p', '--prefix',
        dest='postfix', action='store_false', default=None,
        help='BRAND is prefixed "WM-XXXXXXXX" (default)')
    prefix_group.add_argument('-P', '--postfix',
        dest='postfix', action='store_true', default=None,
        help='BRAND is postfixed "XXXXXXXX-WM"')

    parser.add_argument('-2', dest='count', action='store_const', const=2, default=1,
        required=False, help='generate two V_CODE keys')

    args = parser.parse_args(args)
    if args.brand is None:
        if args.postfix is not None:
            parser.error('BRAND is required with prefix/postfix option')
        if args.count != 1 and args.nobrand is None:
            parser.error('BRAND is expected')
        if args.nobrand is None:
            parser.exit(1, parser.print_help()) #parser.print_usage()

    for _ in range(args.count):
        print(rand_vcode(None if args.nobrand else args.brand, args.postfix))
    parser.exit(0)


if __name__ == '__main__':
    exit(main())
