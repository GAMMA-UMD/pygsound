import unittest
import pygsound as ps
import multiprocessing
import numpy as np


def check_ir(samples):
    assert np.argmax(np.fabs(samples)) > 0, "Max val is at beginning"
    assert np.max(np.fabs(samples)) > 0, "IR max is zero"
    assert ~np.isnan(samples).any(), "IR contains NAN"


class MainTest(unittest.TestCase):
    def test_exception(self):
        self.assertRaises(AssertionError, check_ir, [1, 0, 0])
        self.assertRaises(AssertionError, check_ir, [0, 0, 0])
        self.assertRaises(AssertionError, check_ir, [np.nan, 0, 0])

    def test_rir(self):
        seed = 0
        np.random.seed(seed)

        low = 0.5
        high = 0.99
        N = 10
        cnt = 0
        margin = 0.1

        roomdims = np.random.uniform(0.5, 10.0, (N, 3)).tolist()
        lis_locs = [[np.random.uniform(margin, x - margin) for x in roomdim] for roomdim in roomdims]
        src_locs = [[np.random.uniform(margin, x - margin) for x in roomdim] for roomdim in roomdims]
        alphas = 1 - np.random.uniform(low, high, (N, ))
        while cnt < N:
            alpha = alphas[cnt]
            tasks = [[src_locs[cnt], lis_locs[cnt]]]
            compute_scene_ir_absorb(roomdims[cnt], tasks, alpha)
            cnt += 1

    def test_rir_pairs(self):
        roomdim = [10, 10, 10]
        src_locs = [[0.5, 0.5, 0.5], [9.5, 9.5, 9.5]]
        lis_locs = [[2.5, 0.5, 0.5], [5.0, 5.0, 5.0], [9.5, 0.5, 0.5]]
        alpha = 0.5

        mesh = ps.createbox(roomdim[0], roomdim[1], roomdim[2], alpha, 0.5)

        ctx = ps.Context()
        ctx.diffuse_count = 2000
        ctx.specular_count = 2000
        ctx.threads_count = min(multiprocessing.cpu_count(), 8)

        scene = ps.Scene()
        scene.setMesh(mesh)

        channel = ps.ChannelLayoutType.mono
        ctx.channel_type = channel
        ctx.sample_rate = 16000

        res = scene.computeIRPairs(src_locs, lis_locs, ctx)
        for src_ir in res['samples']:
            for lis_ir in src_ir:
                check_ir(lis_ir)


def compute_scene_ir_absorb(roomdim, tasks, r):
    # Initialize scene mesh
    try:
        mesh = ps.createbox(roomdim[0], roomdim[1], roomdim[2], r, 0.5)
    except Exception as e:
        print(str(e))

    ctx = ps.Context()
    ctx.diffuse_count = 2000
    ctx.specular_count = 2000
    ctx.threads_count = min(multiprocessing.cpu_count(), 8)

    scene = ps.Scene()
    scene.setMesh(mesh)

    channel = ps.ChannelLayoutType.mono
    ctx.channel_type = channel
    ctx.sample_rate = 16000

    for task in tasks:
        src_coord = task[0]
        lis_coord = task[1]

        src = ps.Source(src_coord)
        src.radius = 0.01

        lis = ps.Listener(lis_coord)
        lis.radius = 0.01

        res = scene.computeIR(src, lis, ctx)
        res['samples'] = np.atleast_2d(res['samples'])
        check_ir(res['samples'])


if __name__ == "__main__":
    unittest.main()
