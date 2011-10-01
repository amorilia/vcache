import nose.tools
import vcache

def get_obj_faces(filename):
    with open(filename, "rt") as stream:
        for line in stream:
            if line.startswith('f'):
                yield [int(index) for index in line[2:].split()]

def test_optimize_simple():
    faces = [
        [1, 5, 2],
        [5, 2, 6],
        [5, 9, 6],
        [9, 6, 10],
        [9, 13, 10],
        [13, 10, 14],
        [0, 4, 1],
        [4, 1, 5],
        [4, 8, 5],
        [8, 5, 9],
        [8, 12, 9],
        [12, 9, 13],
        [2, 6, 3],
        [6, 3, 7],
        [6, 10, 7],
        [10, 7, 11],
        [10, 14, 11],
        [14, 11, 15],
        ]
    opt_faces = [
        [0, 4, 1],
        [1, 5, 4],
        [1, 5, 2],
        [4, 8, 5],
        [2, 6, 5],
        [2, 6, 3],
        [3, 7, 6],
        [5, 9, 8],
        [5, 9, 6],
        [8, 12, 9],
        [9, 13, 12],
        [6, 10, 7],
        [6, 10, 9],
        [9, 13, 10],
        [7, 11, 10],
        [10, 14, 13],
        [10, 14, 11],
        [11, 15, 14],
        ]
    tvr = vcache.get_transform_to_vertex_ratio(faces, 8);
    nose.tools.assert_equal(tvr, (24, 16));
    tvr = vcache.get_transform_to_vertex_ratio(opt_faces, 8);
    nose.tools.assert_equal(tvr, (16, 16));
    nose.tools.assert_equal(opt_faces, vcache.get_cache_optimized_faces(faces))

def test_optimize_suzanne():
    faces = list(get_obj_faces("test/obj/suzanne.obj"))
    opt_faces = vcache.get_cache_optimized_faces(faces)
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(faces, 32),
        (3092, 2012))
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(opt_faces, 32),
        (2637, 2012))

def test_optimize_grid_48x48():
    faces = list(get_obj_faces("test/obj/grid-48x48.obj"))
    opt_faces = vcache.get_cache_optimized_faces(faces)
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(faces, 32),
        (4052, 2304))
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(opt_faces, 32),
        (3021, 2304))
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(faces, 16),
        (4305, 2304))
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(opt_faces, 16),
        (3058, 2304))
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(faces, 8),
        (4420, 2304))
    nose.tools.assert_equal(
        vcache.get_transform_to_vertex_ratio(opt_faces, 8),
        (3362, 2304))
