# Generado por LLM; para mejor visualización de grafos en formato .mtx.

import sys

def convert_mtx_to_dot(input_mtx, output_dot):
    with open(input_mtx, 'r') as f:
        lines = f.readlines()

    with open(output_dot, 'w') as f:
        # 'digraph' tells Graphviz this is a directed graph
        f.write("digraph G {\n")
        f.write("  node [shape=circle, style=filled, fillcolor=lightblue];\n")
        f.write("  edge [fontsize=10, fontcolor=red];\n") # Red weights for readability
        f.write("  overlap=false;\n")

        header_skipped = False
        for line in lines:
            line = line.strip()
            # Skip MatrixMarket comments
            if line.startswith('%') or not line:
                continue
            
            parts = line.split()
            
            # Skip the first line containing (nodes, nodes, edges)
            if not header_skipped and len(parts) == 3:
                header_skipped = True
                continue
            
            # Write the edge
            if len(parts) >= 3:
                u, v, w = parts[0], parts[1], parts[2]
                f.write(f'  "{u}" -> "{v}" [label="{w}"];\n')

        f.write("}\n")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python mtx_to_dot.py input.mtx output.dot")
    else:
        convert_mtx_to_dot(sys.argv[1], sys.argv[2])
        print(f"Successfully converted {sys.argv[1]} to {sys.argv[2]}")