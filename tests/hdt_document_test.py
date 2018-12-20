# hdt_document_test.py
# Author: Thomas MINIER - MIT License 2017-2018
import pytest
import hdt
from hdt import HDTDocument
import os

path = "tests/test.hdt"
document = HDTDocument(path)
nbTotalTriples = 132


def test_missing_file():
    with pytest.raises(RuntimeError):
        HDTDocument("/home/dtrump/wall.hdt")

def test_file_path():
    assert document.file_path == path


def test_total_triples():
    assert document.total_triples == nbTotalTriples
    assert len(document) == nbTotalTriples


def test_nb_subjects():
    assert document.nb_subjects == 4


def tests_nb_predicates():
    assert document.nb_predicates == 3


def tests_nb_objects():
    assert document.nb_objects == 112


def tests_nb_shared():
    assert document.nb_shared == 0

def test_seach_missing_resource():
    (triples, triplesCard) = document.search_triples("missingtriple", "", "")
    assert triplesCard == 0

def test_ids_to_string():
    (triples, triplesCard) = document.search_triples("", "", "")
    (ids, idsCard) = document.search_triples_ids("", "", "")
    assert triplesCard == idsCard
    assert triplesCard == nbTotalTriples
    for subj, pred, obj in triples:
        sid, pid, oid = next(ids)
        s, p, o = document.tripleid_to_string(sid, pid, oid)
        assert subj == s
        assert pred == p
        assert obj == o

def test_generate():
    path = "tests"

    #dir_path = os.path.dirname(os.path.realpath(path)) + "/" + path
    dir_path = os.getcwd() + "/tests"
    src_path = dir_path + "/test2.turtle"
    dest_path = dir_path + "/test2.hdt"

    doc = hdt.generate_hdt(src_path, "aff4://foo")
    retcode = doc.save_to_hdt(dest_path)
    assert retcode == 0

    document = HDTDocument(dest_path)

    (triples, triplesCard) = document.search_triples("missingtriple", "", "")
    assert triplesCard == 0

    (triples, triplesCard) = document.search_triples("", "", "")
    assert triplesCard == 12

    (triples, triplesCard) = document.search_triples("aff4://5aea2dd0-32b4-4c61-a9db-677654be6f83//test_images/AFF4-L/dream.txt", "", "")
    assert triplesCard == 12

    os.unlink(dest_path)
