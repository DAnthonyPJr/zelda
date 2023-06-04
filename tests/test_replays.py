import json
import os
import re
import shutil
import subprocess
import sys
import unittest
from pathlib import Path
from common import ReplayTestResults

script_dir = Path(os.path.dirname(os.path.realpath(__file__)))
root_dir = script_dir.parent
test_dir = root_dir / '.tmp/test_replays'
failing_replay = test_dir / 'failing.zplay'
output_dir = test_dir / 'output'


def create_test_replay(contents):
    if test_dir.exists():
        shutil.rmtree(test_dir)
    test_dir.mkdir(parents=True)
    failing_replay.write_text(contents)


def get_snapshots():
    paths = [s.relative_to(output_dir).as_posix()
             for s in output_dir.rglob('*.png')]
    return sorted(paths, key=lambda k:
                  int(re.match(r'.*\.zplay\.(\d+)', k).group(1)))


class TestReplays(unittest.TestCase):

    def setUp(self):
        self.maxDiff = None

    def run_replay(self):
        args = [
            sys.executable,
            root_dir / 'tests/run_replay_tests.py',
            '--test_results', output_dir,
        ]
        if 'BUILD_FOLDER' in os.environ:
            args.append('--build_folder')
            args.append(os.environ['BUILD_FOLDER'])
        args.append(failing_replay)
        output = subprocess.run(args, stdout=subprocess.DEVNULL)
        self.assertEqual(output.returncode, 1)

        test_results_path = test_dir / 'output/test_results.json'
        test_results_json = json.loads(test_results_path.read_text('utf-8'))
        return ReplayTestResults(**test_results_json)

    def test_failing_replay_different_gfx_step(self):
        failing_replay_contents = (
            root_dir / 'tests/replays/classic_1st_lvl1.zplay').read_text()
        failing_replay_contents = failing_replay_contents.replace(
            'C 549 g H!V', 'C 549 g blah')
        failing_replay_contents = failing_replay_contents.replace(
            'C 1574 g H@:', 'C 1574 g blah')
        create_test_replay(failing_replay_contents)

        test_results = self.run_replay()
        result = test_results.runs[0][0]
        self.assertEqual(result.name, 'failing.zplay')
        self.assertEqual(result.success, False)
        self.assertEqual(result.failing_frame, 549)
        self.assertEqual(get_snapshots(), [
            '0/failing/failing.zplay.539.png',
            '0/failing/failing.zplay.540.png',
            '0/failing/failing.zplay.541.png',
            '0/failing/failing.zplay.542.png',
            '0/failing/failing.zplay.543.png',
            '0/failing/failing.zplay.544.png',
            '0/failing/failing.zplay.545.png',
            '0/failing/failing.zplay.546.png',
            '0/failing/failing.zplay.547.png',
            '0/failing/failing.zplay.548.png',
            '0/failing/failing.zplay.549-unexpected.png',
            '0/failing/failing.zplay.550.png',
            '0/failing/failing.zplay.551.png',
            '0/failing/failing.zplay.552.png',
            '0/failing/failing.zplay.553.png',
            '0/failing/failing.zplay.554.png',
            '0/failing/failing.zplay.555.png',
            '0/failing/failing.zplay.556.png',
            '0/failing/failing.zplay.557.png',
            '0/failing/failing.zplay.558.png',
            '0/failing/failing.zplay.1560.png',
            '0/failing/failing.zplay.1561.png',
            '0/failing/failing.zplay.1562.png',
            '0/failing/failing.zplay.1564.png',
            '0/failing/failing.zplay.1566.png',
            '0/failing/failing.zplay.1567.png',
            '0/failing/failing.zplay.1568.png',
            '0/failing/failing.zplay.1570.png',
            '0/failing/failing.zplay.1572.png',
            '0/failing/failing.zplay.1573.png',
            '0/failing/failing.zplay.1574-unexpected.png',
            '0/failing/failing.zplay.1575.png',
            '0/failing/failing.zplay.1576.png',
            '0/failing/failing.zplay.1577.png',
            '0/failing/failing.zplay.1578.png',
            '0/failing/failing.zplay.1579.png',
            '0/failing/failing.zplay.1580.png',
            '0/failing/failing.zplay.1581.png',
            '0/failing/failing.zplay.1582.png',
            '0/failing/failing.zplay.1583.png',
        ])

    def test_failing_replay_missing_gfx_step(self):
        failing_replay_contents = (
            root_dir / 'tests/replays/classic_1st_lvl1.zplay').read_text()
        failing_replay_contents = failing_replay_contents.replace(
            'C 549 g H!V\n', '')
        failing_replay_contents = failing_replay_contents.replace(
            'C 1574 g H@:\n', '')
        create_test_replay(failing_replay_contents)

        test_results = self.run_replay()
        result = test_results.runs[0][0]
        self.assertEqual(result.name, 'failing.zplay')
        self.assertEqual(result.success, False)
        self.assertEqual(result.failing_frame, 550)
        snapshots = get_snapshots()
        self.assertEqual(len(snapshots), 40)
        self.assertEqual([s for s in snapshots if 'unexpected.png' in s], [
            '0/failing/failing.zplay.549-unexpected.png',
            '0/failing/failing.zplay.1574-unexpected.png',
        ])

    def test_never_ending_failing_replay(self):
        failing_replay_contents = (
            root_dir / 'tests/replays/classic_1st_lvl1.zplay').read_text()
        lines = failing_replay_contents.splitlines()
        failing_replay_contents = '\n'.join(
            l for l in lines if not l.startswith('D') and not l.startswith('U'))
        create_test_replay(failing_replay_contents)

        test_results = self.run_replay()
        result = test_results.runs[0][0]
        self.assertEqual(result.name, 'failing.zplay')
        self.assertEqual(result.success, False)
        self.assertEqual(result.failing_frame, 1)
        snapshots = get_snapshots()
        self.assertEqual(len(snapshots), 1197)
        self.assertEqual(
            len([s for s in snapshots if 'unexpected.png' in s]), 916)


if __name__ == '__main__':
    unittest.main()