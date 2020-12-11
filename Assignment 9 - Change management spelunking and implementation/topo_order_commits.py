import sys
import os
import zlib


def topo_order_commits():
    main_function()


def get_topo_ordered_commits(commit_nodes, root_hashes):
    order = []
    visited = set()
    temp_stack = []
    stack = sorted(root_hashes)
    
    while stack:
        top = stack[-1]
        visited.add(top)
        children = []
        for c in sorted(find_node(top, commit_nodes).children):
            if c not in visited:
                children.append(c)
        if children:
            stack.append(children[0])
        else:
            order.append(stack.pop())   
        
    return order


def find_git(dir):
    while(os.path.isdir(dir+"/.git") == False):
        if dir == "/":
            sys.exit("Not inside a git repository")
        dir = os.path.abspath(os.path.join(dir, os.pardir))
    return dir+"/.git"


def get_branches(git_dir):
    branches = {}
    branch_dir = git_dir+"/refs/heads"
    for (root, dirnames, filenames) in os.walk(branch_dir):
        for file in filenames:
            file = os.path.join(root, file)
            index = file.index("/.git/refs/heads/")
            hashval = open(file, 'rb').read().decode("utf-8", "ignore")
            
            if hashval[:len(hashval)-1] in branches:
                branches[hashval[:len(hashval)-1]].append(file[index+17:]) 
            else:
                temp=[file[index+17:]]
                branches[hashval[:len(hashval)-1]] = temp
    
    return branches


class CommitNode:
    def __init__(self, commit_hash):
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()


def decompress(filename):
    compressed_contents = open(filename, 'rb').read()
    decompressed_contents = zlib.decompress(compressed_contents)
    return decompressed_contents
    
def find_parents(commit_hash, git_dir):
    parents = set()
    for (root, dirnames, filenames) in os.walk(git_dir+"/objects/"+commit_hash[:2]):
        for file in filenames:
            path = os.path.join(root, file)
            if(path == "/Users/dhakshin/Desktop/35L/Assignment9/.git/objects/.DS_Store"):
                continue
            contents = decompress(path).decode("utf-8", "ignore")
            if contents.split()[0] == "commit":
                index = path.index("/.git/objects")
                hashid = path[index+14:]
                hashid = hashid.replace("/", "")
                if hashid == commit_hash:
                    lines = contents.split("\n")
                    for line in lines:
                        if len(line) != 0 and line.split()[0] == "parent":
                            parents.add(line.split()[1])
    return parents


def find_node(commit_hash, commit_nodes):
    for commit in commit_nodes:
        if commit.commit_hash == commit_hash:
            return commit
    return


def build_commit_graph(git_dir, local_branch_heads):
    commit_nodes = []
    root_hashes = []
    visited = set()  # this might be a problem: since 2 branches might point to same commit?
    stack = list(local_branch_heads.keys())

    while stack:
        commit_hash = stack.pop()
        # print(commit_hash)
        if commit_hash in visited:
            continue
        elif not find_node(commit_hash, commit_nodes):
            commit_nodes.append(CommitNode(commit_hash))
        visited.add(commit_hash)
        commit_node = find_node(commit_hash, commit_nodes)

        commit_node.parents = find_parents(commit_hash, git_dir)
        if not commit_node.parents:
            root_hashes.append(commit_hash)
        for p in commit_node.parents:
            if p not in visited:
                stack.append(p)
            if not find_node(p, commit_nodes):
                p_node = CommitNode(p)
                p_node.children.add(commit_hash)
                commit_nodes.append(p_node)
            else:
                p_node = find_node(p, commit_nodes)
                p_node.children.add(commit_hash)
    return commit_nodes, root_hashes


def print_topo_ordered_commits_with_branch_names(commit_nodes, topo_ordered_commits, head_to_branches):
    jumped = False
    for i in range(len(topo_ordered_commits)):
        commit_hash = topo_ordered_commits[i]
        if jumped:
            jumped = False
            sticky_hash = ' '.join(sorted(commit_nodes[commit_hash].children))
            print(f'={sticky_hash}')
        branches = head_to_branches[commit_hash] if commit_hash in head_to_branches else [
        ]
        print(commit_hash + (' ' + ' '.join(branches) if branches else ''))
        if i+1 < len(topo_ordered_commits) and topo_ordered_commits[i+1] not in commit_nodes[commit_hash].parents:
            jumped = True
            sticky_hash = ' '.join(sorted(commit_nodes[commit_hash].parents))
            print(f'{sticky_hash}=\n')


def main_function():
    git_dir = find_git(os.getcwd())
    git_branches = get_branches(git_dir)
    commit_nodes, root_hashes = build_commit_graph(git_dir, git_branches)
    topo_commits = get_topo_ordered_commits(commit_nodes, root_hashes)
    final_commit_nodes = {}
    for x in commit_nodes:
        final_commit_nodes[x.commit_hash] = x
    
    print_topo_ordered_commits_with_branch_names(final_commit_nodes, topo_commits, git_branches)



if __name__ == '__main__':
    topo_order_commits()
