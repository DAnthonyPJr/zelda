import argparse
import re
import subprocess
from dataclasses import dataclass
from typing import Dict, List
from git_hooks.common import valid_types, valid_scopes

parser = argparse.ArgumentParser()
parser.add_argument(
    '--format', choices=['plaintext', 'markdown'], default='plaintext')
parser.add_argument('--from')
parser.add_argument('--to', default='HEAD')

args = parser.parse_args()
from_sha = getattr(args, 'from', None)

commit_url_prefix = 'https://github.com/ArmageddonGames/ZQuestClassic/commit'


@dataclass
class Commit:
    type: str
    scope: str
    short_hash: str
    hash: str
    oneline: str
    body: str

    def scope_and_oneline(self):
        if self.scope:
            return f'{self.scope}: {self.oneline}'
        else:
            return self.oneline


def parse_scope_and_type(subject: str):
    match = re.match(r'(\w+)\((\w+)\): (.+)', subject)
    if match:
        type, scope, oneline = match.groups()
        return type, scope, oneline

    match = re.match(r'(\w+): (.+)', subject)
    if not match:
        return 'misc', None, subject

    type, oneline = match.groups()
    return type, None, oneline


def get_scope_index(scope: str):
    if scope in valid_scopes:
        return valid_scopes.index(scope)
    else:
        return -1


def get_type_label(type: str):
    match type:
        case 'feat': return 'Features'
        case 'fix': return 'Bug Fixes'
        case 'docs': return 'Documentation'
        case 'chore': return 'Chores'
        case 'refactor': return 'Refactors'
        case 'test': return 'Tests'
        case 'ci': return 'CI'
        case 'misc': return 'Misc.'
        case _: return type.capitalize()


def get_scope_label(scope: str):
    match scope:
        case 'zc': return 'Player'
        case 'zq': return 'Editor'
        case 'zscript': return 'ZScript'
        case 'launcher': return 'ZLauncher'
        case 'zconsole': return 'ZConsole'
        case _: return scope.capitalize()


def generate_changelog(commits_by_type: Dict[str, List[Commit]], format: str):
    lines = []

    if format == 'markdown':
        for type, commits in commits_by_type.items():
            label = get_type_label(type)
            lines.append(f'# {label}\n')

            commits_by_scope: Dict[str, List[Commit]] = {}
            for commit in commits:
                by_scope = commits_by_scope.get(commit.scope, [])
                commits_by_scope[commit.scope] = by_scope
                by_scope.append(commit)

            for scope, commits in commits_by_scope.items():
                if scope:
                    label = get_scope_label(scope)
                    lines.append(f'### {label}\n')
                for c in commits:
                    link = f'[`{c.short_hash}`]({commit_url_prefix}/{c.hash})'
                    lines.append(f'- {c.oneline} {link}')
                lines.append('')
    elif format == 'plaintext':
        for type, commits in commits_by_type.items():
            label = get_type_label(type)
            lines.append(f'# {label}\n')
            for c in commits:
                lines.append(f'{c.scope_and_oneline()}')
            lines.append('')

    return '\n'.join(lines)


if from_sha:
    branch = from_sha
else:
    branch = subprocess.check_output(
        'git describe --tags --abbrev=0', shell=True, encoding='utf-8').strip()

commits_text = subprocess.check_output(
    f'git log {branch}...{args.to} --format="%h %H %s"', shell=True, encoding='utf-8').strip()

commits: List[Commit] = []
for commit_text in commits_text.splitlines():
    short_hash, hash, subject = commit_text.split(' ', 2)
    # TODO: not used for anything yet.
    # body = subprocess.check_output(
    #     f'git log -1 {hash} --format="%b"', shell=True, encoding='utf-8').strip()
    body = None
    type, scope, oneline = parse_scope_and_type(subject)
    commits.append(Commit(type, scope, short_hash, hash, oneline, body))


commits_by_type: Dict[str, List[Commit]] = {}
for type in valid_types:
    commits_by_type[type] = []


for commit in commits:
    by_type = commits_by_type.get(commit.type, [])
    commits_by_type[commit.type] = by_type
    by_type.append(commit)

to_remove = []
for type, commits in commits_by_type.items():
    if not commits:
        to_remove.append(type)

    commits.sort(key=lambda c: get_scope_index(c.scope))

for key in to_remove:
    del commits_by_type[key]

print(generate_changelog(commits_by_type, args.format))
