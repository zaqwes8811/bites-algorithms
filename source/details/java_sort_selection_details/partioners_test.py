import unittest
import random

from stanford_algoritms_part1.build_blocks import partioners


class TestSequenceFunctions(unittest.TestCase):
    def setUp(self):
        self.seq = range(10)

    def test_shuffle(self):
        # make sure the shuffled sequence does not lose any elements
        random.shuffle(self.seq)
        self.seq.sort()
        self.assertEqual(self.seq, range(10))

    def test_part(self):
        in_array = [3, 8, 2, 5, 1, 4, 7, 6]
        partitioner = partioners.Partitioner()
        partitioner.partition(in_array)
        self.assertEqual(3, in_array[2])
        self.assertTrue(min(in_array[3:]) > 3)