import sys
import os

sys.path.append("/opt/Coreform-Cubit-2022.12/bin")

import cubit

file_path = "/home/bill/projects/libmeshSkinning/Meshes/coil_fo_hive_rot_tet.e"
dir = os.path.dirname(file_path) + "/"
output_file_name = ((os.path.basename(file_path)).split(sep='surf'))[0] + "bound.e"

# export_name 
cubit.init(['cubit','-nojournal'])
cubit.cmd('set echo on')

import_command = 'import mesh geometry "' + file_path +  '" feature_angle 135.00  merge'
print(import_command)
cubit.cmd(import_command)

