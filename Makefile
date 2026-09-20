.PHONY: test check clean

test:
	python3 -m unittest discover -s tests -v

check:
	bash tools/check_all.sh

clean:
	rm -rf build dist *.egg-info src/*.egg-info .pytest_cache
	find . -name __pycache__ -type d -exec rm -rf {} +
