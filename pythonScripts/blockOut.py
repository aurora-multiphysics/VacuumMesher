import sys

sys.path.append("/opt/Coreform-Cubit-2022.4/bin")

import cubit

cubit.init(['cubit','-nojournal'])
cubit.cmd('import mesh geometry "/home/bill/projects/libmesh-skinning/Meshes/chimeraCsut.e" feature_angle 135.00  merge ')
cubit.cmd('undo group begin')
for i in cubit.get_block_id_list():
    cubit.cmd('set exodus netcdf4 off')
    cubit.cmd('set large exodus file off')
    name = 'export mesh "/home/bill/projects/libmesh-skinning/Meshes/chimBlocks/block' + str(i) + '.e"  block ' + str(i) + ' overwrite'
    cubit.cmd(name)
