SHELL := /bin/bash
.SHELLFLAGS := -eu -o pipefail -c
.ONESHELL:

VENV := .venv
PYTHON := $(VENV)/bin/python

.PHONY: install run clean full_clean lint debug

install:
	@uv venv $(VENV)
	@echo ".venv created"
	@source $(VENV)/bin/activate
	@uv pip install -e .
	@deactivate
	@echo "needed dependencies installed"

run:
	@source $(VENV)/bin/activate
	@python -m src
	@deactivate

clean:
	@rm -rf src/__pycache__

full_clean:
	@rm -rf src/__pycache__ $(VENV)

lint:
	@source $(VENV)/bin/activate
	@flake8 .
	@mypy . --warn-return-any \
		--warn-unused-ignores \
		--ignore-missing-imports \
		--disallow-untyped-defs \
		--check-untyped-defs
	@deactivate

debug:
	@source $(VENV)/bin/activate
	@python -m pdb src/__main__.py
	@deactivate