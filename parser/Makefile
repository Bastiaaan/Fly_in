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
	@rm -rf src/fly_in_map_parser.egg-info
	@rm -rf .mypy_cache

full_clean:
	@rm -rf src/__pycache__ $(VENV)
	@rm -rf src/fly_in_map_parser.egg-info

lint:
	@source $(VENV)/bin/activate
	@flake8 src
	@mypy src --warn-return-any \
		--warn-unused-ignores \
		--ignore-missing-imports \
		--disallow-untyped-defs \
		--check-untyped-defs
	@deactivate

debug:
	@source $(VENV)/bin/activate
	@python -m pdb src/__main__.py
	@deactivate