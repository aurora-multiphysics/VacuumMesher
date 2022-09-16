import sys

sys.path.append("/opt/Coreform-Cubit-2022.4/bin")

import cubit

cubit.init(['cubit','-nojournal'])
cubit.cmd('set echo on')
cubit.cmd('brick x 3 y 3 z 3')
cubit.cmd('surface all copy')
cubit.cmd('delete volume 0')
cubit.cmd('surface all scheme trimesh')
cubit.cmd('mesh surface all')
cubit.cmd('block 2 add surface all')
cubit.cmd('import mesh geometry "/home/bill/projects/libmesh-skinning/Meshes/chimBlocks/skinnedBlocks/block1_surface.e" feature_angle 135.00  merge ')
cubit.cmd('set exodus netcdf4 off')
cubit.cmd('set large exodus file off')
name = 'export mesh "/home/bill/projects/libmesh-skinning/Meshes/chimBlocks/skins.e" overwrite'
cubit.cmd(name)
# cubit.
