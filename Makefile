.PHONY: test test-fast check clean functional-verification functional-verification-controls

test:
	python3 -m unittest discover -s tests -v

test-fast:
	bash tools/test_fast.sh

check:
	bash tools/check_all.sh

# T1 item 5 functional evidence (issue #150).  Full/wave-gate lane only: it is
# NOT part of test-fast (DR-0009) and takes tens of minutes of Icarus time
# (2 parallel playbacks).  Creates a throwaway .venv-fv in this checkout; no
# host tool is installed.  See docs/FUNCTIONAL-VERIFICATION.md.
FV_VENV ?= .venv-fv
FV_PINS = "klayout-tools==0.7.0" "cocotb==2.1.0"

$(FV_VENV)/bin/klt:
	uv venv $(FV_VENV)
	uv pip install --python $(FV_VENV)/bin/python $(FV_PINS)

functional-verification: $(FV_VENV)/bin/klt
	mkdir -p build
	python3 tools/functional_verification.py --venv $(FV_VENV)

functional-verification-controls: $(FV_VENV)/bin/klt
	mkdir -p build
	python3 tools/functional_verification.py --venv $(FV_VENV) --control H07_MUTATE_GLITCH
	python3 tools/functional_verification.py --venv $(FV_VENV) --control H07_MUTATE_SUM_ORDER

clean:
	rm -rf build dist *.egg-info src/*.egg-info .pytest_cache
	find . -name __pycache__ -type d -exec rm -rf {} +
