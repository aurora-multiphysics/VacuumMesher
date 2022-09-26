import sys
import os

sys.path.append("/opt/Coreform-Cubit-2022.4/bin")

import cubit

# file_path = input("Please enter file path:")
# file_name = input("Enter filename:")
file_path = "/home/bill/projects/libmesh-skinning/Meshes/chimBlocks/skinnedBlocks/"
file_name = "chimNoLegsSkin.e"
# output_file_name = input("Please enter an output filename:")
# output_file_path = input("Please enter the output filepath")
output_file_name = "block6_surgery_test_bound.e"
output_file_path = "/home/bill/projects/libmesh-skinning/Meshes/chimBlocks/skinnedBlocks/withBound/"
file_name = file_path + file_name
output_file_name = output_file_path + output_file_name

# export_name 
cubit.init(['cubit','-nojournal'])
cubit.cmd('set echo on')
cubit.cmd('brick x 3 y 3 z 3')
cubit.cmd('surface all copy')
cubit.cmd('del vol 1')
cubit.cmd('surface all scheme trimesh')
cubit.cmd('mesh surface all')
cubit.cmd('block 2 add surface all')

import_command = 'import mesh geometry "' + file_name +  '" feature_angle 135.00  merge '
print(import_command)
cubit.cmd(import_command)
cubit.cmd('set exodus netcdf4 off')
cubit.cmd('set large exodus file off')

export_command = 'export mesh "' + output_file_name + '"  overwrite'
print(export_command)
cubit.cmd(export_command)
cubit.cmd('reset')
