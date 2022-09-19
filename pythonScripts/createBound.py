import sys

sys.path.append("/opt/Coreform-Cubit-2022.4/bin")

import cubit


block_nums = [1, 2, 3, 5, 6]

for i in block_nums:
    cubit.init(['cubit','-nojournal'])
    cubit.cmd('set echo on')
    cubit.cmd('brick x 3 y 3 z 3')
    cubit.cmd('surface all copy')
    cubit.cmd('del vol 1')
    cubit.cmd('surface all scheme trimesh')
    cubit.cmd('mesh surface all')
    cubit.cmd('block 2 add surface all')
    
    print(i)
    import_name = 'import mesh geometry "/home/bill/projects/libmesh-skinning/Meshes/chimBlocks/skinnedBlocks/block' + str(i) + '_surface.e" feature_angle 135.00  merge '
    
    cubit.cmd(import_name)
    cubit.cmd('set exodus netcdf4 off')
    cubit.cmd('set large exodus file off')
    
    export_name = 'export mesh "/home/bill/projects/libmesh-skinning/Meshes/chimBlocks/skinnedBlocks/withBound/block' + str(i) + 'withBound.e" overwrite'
    print(export_name)
    cubit.cmd(export_name)
    cubit.cmd('reset')
