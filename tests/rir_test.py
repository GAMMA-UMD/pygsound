import unittest
import pygsound as ps
import multiprocessing
import numpy as np


class MainTest(unittest.TestCase):
    def test_rir(self):
        seed = 0
        np.random.seed(seed)

        low = 0.5
        high = 0.99
        N = 10
        cnt = 0
        bad_cnt = 0
        margin = 0.1
        bad_configs = []

        roomdims = np.random.uniform(0.5, 10.0, (N, 3)).tolist()
        lis_locs = [[np.random.uniform(margin, x - margin) for x in roomdim] for roomdim in roomdims]
        src_locs = [[np.random.uniform(margin, x - margin) for x in roomdim] for roomdim in roomdims]
        alphas = 1 - np.random.uniform(low, high, (N, ))
        while (cnt < N):
            alpha = alphas[cnt]
            tasks = [[src_locs[cnt], lis_locs[cnt]]]
            status = compute_scene_ir_absorb(roomdims[cnt], tasks, alpha)
            self.assertEqual(status, 0)
            if status:
                bad_cnt += 1
                bad_configs.append([cnt] + roomdims[cnt] + src_locs[cnt] + lis_locs[cnt] + [alpha])
            cnt += 1
            print('{}/{} bad IR'.format(bad_cnt, cnt))

    def test_rir_pairs(self):
        roomdim = [10, 10, 10]
        lis_locs = [[0.5, 0.5, 0.5], [9.5, 9.5, 9.5]]
        src_locs = [[2.5, 0.5, 0.5], [5.0, 5.0, 5.0], [9.5, 0.5, 0.5]]
        alpha = 0.5

        mesh = ps.createbox(roomdim[0], roomdim[1], roomdim[2], alpha, 0.5)

        ctx = ps.Context()
        ctx.diffuse_count = 20
        ctx.specular_count = 20
        ctx.threads_count = min(multiprocessing.cpu_count(), 8)

        scene = ps.Scene()
        scene.setMesh(mesh)

        channel = ps.ChannelLayoutType.mono
        ctx.channel_type = channel
        ctx.sample_rate = 16000

        res = scene.computeIRPairs(src_locs, lis_locs, ctx)
        # TODO: check results
        pass


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

    status = 0

    for task in tasks:
        src_coord = task[0]
        lis_coord = task[1]

        src = ps.Source(src_coord)
        src.radius = 0.01

        lis = ps.Listener(lis_coord)
        lis.radius = 0.01
        # lis.channel_layout_type = ps.ChannelLayoutType.mono

        a = np.array(src_coord)
        b = np.array(lis_coord)
        dist = np.linalg.norm(a - b)
        direct_idx = int(ctx.sample_rate * dist / 343)
        res = scene.computeIR(src, lis, ctx)
        res['samples'] = np.atleast_2d(res['samples'])
        if (np.argmax(np.fabs(res['samples'])) == 0):
            status = 1
        elif (np.max(np.fabs(res['samples'])) == 0):
            status = 2
        elif (np.isnan(res['samples']).any()):
            status = 3

    return status


if __name__ == "__main__":
    unittest.main()
