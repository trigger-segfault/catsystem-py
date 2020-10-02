#!/usr/bin/python
# -*- coding: utf-8 -*-
"""CatSystem archive extraction
"""

__version__ = '1.0.0'
__date__    = '2020-09-17'
__author__  = 'Robert Jordan'

__all__ = ['Archive', 'ArchiveEntry', 'registerarc', 'identifyarc', 'openarc']

#######################################################################################

# local module imports
from ._basearc import *

# local archive types (importing registers these types)
from . import Kif, CsPack

