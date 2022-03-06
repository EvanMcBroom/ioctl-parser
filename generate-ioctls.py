#!/usr/bin/env python3
# -*- coding: utf-8 -*-

'''Generates the IOCTLs.ini reference file.'''

import configparser
from datetime import datetime
import pandas as pd
import pathlib
import sys

# Create a pandas data frame of IO control code values
url = r'http://www.ioctls.net/'
tables = pd.read_html(url)
ioctls = tables[0]

# Replace the full path of the header file values with just their file names
ioctls['Header'] = ioctls['Header'].apply(lambda header: pathlib.PurePath(header).name if type(header) == str else '')

# Sort the data frame then generate the INI file
ioctls.sort_values(by=['Header', 'Hex Value'], inplace=True)
config = configparser.ConfigParser()
for header in ioctls['Header'].unique():
    config[header if len(header) else 'Unknown'] = \
        {row['Ioctl']:row['Hex Value'] for index, row in ioctls.loc[ioctls['Header'] == header].iterrows()}
print('# Generated on {}'.format(datetime.now().strftime('%m/%d/%Y %H:%M:%S')))
config.write(sys.stdout)