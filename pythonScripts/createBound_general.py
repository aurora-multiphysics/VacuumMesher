import sys
import os

sys.path.append("/opt/Coreform-Cubit-2022.4/bin")

import cubit

file_path = sys.argv[1]
dir = os.path.dirname(file_path) + "/"
output_file_name = ((os.path.basename(file_path)).split(sep='surf'))[0] + "bound.e"

# export_name 
cubit.init(['cubit','-nojournal'])
cubit.cmd('set echo on')
cubit.cmd('brick x 3 y 3 z 3')
cubit.cmd('surface all copy')
cubit.cmd('del vol 1')
cubit.cmd('surface all scheme trimesh')
cubit.cmd('mesh surface all')
cubit.cmd('block 2 add surface all')

import_command = 'import mesh geometry "' + file_path +  '" feature_angle 135.00  merge '
print(import_command)
cubit.cmd(import_command)
cubit.cmd('set exodus netcdf4 off')
cubit.cmd('set large exodus file off')

export_command = 'export mesh "' + dir + output_file_name + '"  overwrite'
print(export_command)
cubit.cmd(export_command)
cubit.cmd('reset')
