from starid.demo import Demo


def test_data():
    demo = Demo()
    assert demo.check_data()
