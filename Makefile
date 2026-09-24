.PHONY: test test-fast check clean

test:
	python3 -m unittest discover -s tests -v

test-fast:
	bash tools/test_fast.sh

check:
	bash tools/check_all.sh

clean:
	rm -rf build dist *.egg-info src/*.egg-info .pytest_cache
	find . -name __pycache__ -type d -exec rm -rf {} +
