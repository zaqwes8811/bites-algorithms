import unittest
import random

from stanford_algoritms_part1.sort import partioners


class TestSequenceFunctions(unittest.TestCase):
    def setUp(self):
        self.seq = range(10)

    def test_shuffle(self):
        # make sure the shuffled sequence does not lose any elements
        random.shuffle(self.seq)
        self.seq.sort()
        self.assertEqual(self.seq, range(10))

    def test_part(self):
        in_array = []
        partitioner = partioners.PartionerBase()
        partitioner.partition(in_array)