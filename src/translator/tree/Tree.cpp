#include <functional>
#include <iostream>
#include <sstream>
#include <vector>

#include "screen/Screen.h"
#include "translator/Translator.h"

namespace {

struct Node {
  std::wstring content;
  Node* parent = nullptr;
  std::vector<std::unique_ptr<Node>> children;
};

struct Line {
  int spaces = 0;
  std::wstring content;
  Node* tree;
};

std::string PrintUnicode1(std::unique_ptr<Node> tree) {
  std::stringstream output;
  std::function<void(std::unique_ptr<Node>&, std::string)> print_children;
  print_children = [&](std::unique_ptr<Node>& node, std::string prefix) {
    for (int i = 0; i < node->children.size(); ++i) {
      auto& child = node->children[i];
      if (i != node->children.size() - 1) {
        output <<             prefix + " ├─" << to_string(child->content) << std::endl;
        print_children(child, prefix + " │ ");
      } else {
        output <<             prefix + " └─" << to_string(child->content) << std::endl;
        print_children(child, prefix + "   ");
      }
    }
  };

  for(auto& it : tree->children) {
    output << to_string(it->content) << std::endl;
    print_children(it, "");
  }
  return output.str();
}


std::string PrintUnicode2(std::unique_ptr<Node> tree) {
  std::stringstream output;
  std::function<void(std::unique_ptr<Node>&, std::string)> print_children;
  print_children = [&](std::unique_ptr<Node>& node, std::string prefix) {
    for (int i = 0; i < node->children.size(); ++i) {
      auto& child = node->children[i];
      if (i != node->children.size() - 1) {
        output <<             prefix + " ├──" << to_string(child->content) << std::endl;
        print_children(child, prefix + " │  ");
      } else {
        output <<             prefix + " └──" << to_string(child->content) << std::endl;
        print_children(child, prefix + "    ");
      }
    }
  };

  for(auto& it : tree->children) {
    output << to_string(it->content) << std::endl;
    print_children(it, "");
  }
  return output.str();
}

std::string PrintASCII1(std::unique_ptr<Node> tree) {
  std::stringstream output;
  std::function<void(std::unique_ptr<Node>&, std::string)> print_children;
  print_children = [&](std::unique_ptr<Node>& node, std::string prefix) {
    for (int i = 0; i < node->children.size(); ++i) {
      auto& child = node->children[i];
      if (i != node->children.size() - 1) {
        output <<             prefix + " +-" << to_string(child->content) << std::endl;
        print_children(child, prefix + " |  ");
      } else {
        output <<             prefix + " `-" << to_string(child->content) << std::endl;
        print_children(child, prefix + "    ");
      }
    }
  };

  for(auto& it : tree->children) {
    output << to_string(it->content) << std::endl;
    print_children(it, "");
  }
  return output.str();
}

std::string PrintASCII2(std::unique_ptr<Node> tree) {
  std::stringstream output;
  std::function<void(std::unique_ptr<Node>&, std::string)> print_children;
  print_children = [&](std::unique_ptr<Node>& node, std::string prefix) {
    for (int i = 0; i < node->children.size(); ++i) {
      auto& child = node->children[i];
      if (i != node->children.size() - 1) {
        output <<             prefix + " +--" << to_string(child->content) << std::endl;
        print_children(child, prefix + " |  ");
      } else {
        output <<             prefix + " `--" << to_string(child->content) << std::endl;
        print_children(child, prefix + "    ");
      }
    }
  };

  for(auto& it : tree->children) {
    output << to_string(it->content) << std::endl;
    print_children(it, "");
  }
  return output.str();
}

std::string PrintASCII3(std::unique_ptr<Node> tree) {
  std::stringstream output;
  std::function<void(std::unique_ptr<Node>&, std::string)> print_children;
  print_children = [&](std::unique_ptr<Node>& node, std::string prefix) {
    for (int i = 0; i < node->children.size(); ++i) {
      auto& child = node->children[i];
      if (i != node->children.size() - 1) {
        output <<             prefix + " |--" << to_string(child->content) << std::endl;
        print_children(child, prefix + " |  ");
      } else {
        output <<             prefix + " `--" << to_string(child->content) << std::endl;
        print_children(child, prefix + "    ");
      }
    }
  };

  for(auto& it : tree->children) {
    output << to_string(it->content) << std::endl;
    print_children(it, "");
  }
  return output.str();
}

struct DisplayTree {
  int entrance;
  std::vector<std::wstring> content;
};

enum class Align {
  Top,
  Center,
  Bottom,
};

DisplayTree MergeDisplayTree(std::wstring content,
                             std::vector<DisplayTree> children,
                             Align align) {
  std::wstring space_to_add(content.size() + 3, U' ');
  DisplayTree ret;

  // Leaf of the recursion.
  if (children.size() == 0) {
    ret.content.push_back(content);
    ret.entrance = 0;
    return ret;
  }

  // Draw Childrens
  for (auto& child : children) {
    for (auto& line : child.content) {
      ret.content.push_back(space_to_add + line);
    }
  }

  // Draw the current content.
  switch (align) {
    case Align::Top:
      ret.entrance = 0;
      break;
    case Align::Center:
      ret.entrance = ret.content.size() / 2;
      break;
    case Align::Bottom:
      ret.entrance = ret.content.size() - 1;
      break;
  };

  for(int i = 0; i<content.size(); ++i) {
    ret.content[ret.entrance][i] = content[i];
  }

  // Draw vertex.
  int first_entrance = children[0].entrance;
  int last_entrance = 0;
  {
    int y = 0;
    for (auto& child : children) {
      last_entrance = y + child.entrance;
      y += child.content.size();
    }
  }

  int y = 0;
  for(auto& child : children) {
    int start = y;

    // Draw Child vertical connector.
    for (auto& line : child.content) {
      if (y >= first_entrance && y <= last_entrance) {
        ret.content[y][content.size() + 1] = U'│';
      }
      ++y;
    }

    // Refine connector on child entrance points.
    int child_entrance = start + child.entrance;
    auto& connector = ret.content[child_entrance][content.size() + 1];
    if (first_entrance == last_entrance)
      connector = U'─';
    else if (child_entrance == first_entrance)
      connector = U'┌';
    else if (child_entrance < last_entrance)
      connector = U'├';
    else
      connector = U'└';

    // Draw connector to child entrance.
    ret.content[child_entrance][content.size() + 2] = U'─';
  }

  // Draw parent entrance to connector.
  ret.content[ret.entrance][content.size() + 0] = U'─';
  // Refine connector on parent entrance points.
  auto& connector = ret.content[ret.entrance][content.size() + 1];
  switch (connector) {
    case U'─': connector = U'─'; break;
    case U'┌': connector = U'┬'; break;
    case U'├': connector = U'┼'; break;
    case U'└': connector = U'┴'; break;
    case U'│': connector = U'┤'; break;
  }

  return ret;
}

DisplayTree MakeDisplayTree(Node* tree, Align align) {
  std::vector<DisplayTree> children_tree;
  int i = 0;
  for (auto& child : tree->children) {
    children_tree.push_back(MakeDisplayTree(child.get(), align));
  }
  return MergeDisplayTree(tree->content, std::move(children_tree), align);
}

std::string PrintUnicodeRightTop(std::unique_ptr<Node> tree) {
  DisplayTree display = MakeDisplayTree(tree.get(), Align::Top);
  std::stringstream output;
  for(auto& line : display.content) {
    output << to_string(line) << std::endl;
  }
  return output.str();
}

std::string PrintUnicodeRightCenter(std::unique_ptr<Node> tree) {
  DisplayTree display = MakeDisplayTree(tree.get(), Align::Center);
  std::stringstream output;
  for(auto& line : display.content) {
    output << to_string(line) << std::endl;
  }
  return output.str();
}

std::string PrintUnicodeRightBottom(std::unique_ptr<Node> tree) {
  DisplayTree display = MakeDisplayTree(tree.get(), Align::Bottom);
  std::stringstream output;
  for(auto& line : display.content) {
    output << to_string(line) << std::endl;
  }
  return output.str();
}

std::map<std::string, std::function<std::string(std::unique_ptr<Node>)>>
    print_function = {
        {"unicode 1", PrintUnicode1},
        {"unicode 2", PrintUnicode2},
        {"ASCII 1", PrintASCII1},
        {"ASCII 2", PrintASCII2},
        {"ASCII 3", PrintASCII3},
        {"unicode right top", PrintUnicodeRightTop},
        {"unicode right center", PrintUnicodeRightCenter},
        {"unicode right bottom", PrintUnicodeRightBottom},
};

} // namespace

class Tree : public Translator {
 public:
  virtual ~Tree() = default;
  std::string Translate(const std::string& input,
                        const std::string& options_string) override {
    auto options = SerializeOption(options_string);

    // Style.
    std::string style_option = options["style"];

    // Parse the tree.
    std::vector<Line> lines;

    std::string line_text;
    std::stringstream ss(input);
    while (std::getline(ss, line_text)) {
      Line line;
      line.content = to_wstring(line_text);
      while (line.spaces < line.content.size() &&
             (line.content[line.spaces] == U' ' ||
              line.content[line.spaces] == U'\t')) {
        line.spaces++;
      }
      line.content = line.content.substr(line.spaces, -1);
      lines.push_back(line);
    }

    if (lines.size() == 0)
      return "";
    
    // Build the tree.
    auto tree = std::make_unique<Node>();
    for(int i = 0; i<lines.size(); ++i) {
      auto child = std::make_unique<Node>();
      lines[i].tree = child.get();
      child->content = lines[i].content;
      for (int j = i - 1; ; --j) {

        if (j == -1) {
          child->parent = tree.get();
          child->parent->children.push_back(std::move(child));
          break;
        }

        if (lines[j].spaces < lines[i].spaces) {
          child->parent = lines[j].tree;
          child->parent->children.push_back(std::move(child));
          break;
        }
      }
    }

    if (print_function.count(style_option)) {
      return print_function[style_option](std::move(tree));
    } else {
      return print_function["unicode 2"](std::move(tree));
    }
  }
  // ----------------------------------------------
  const char* Name() override;
  const char* Description() override;
  std::vector<OptionDescription> Options() override;
  std::vector<Example> Examples() override;
};

std::unique_ptr<Translator> TreeTranslator() {
  return std::make_unique<Tree>();
}

const char* Tree::Name() {
  return "Tree";
}

const char* Tree::Description() {
  return "Draw a tree";
}

std::vector<Translator::OptionDescription> Tree::Options() {
  return {
      {
        "style",
        "The style of the table.\n"
        "Possible values:\n"
        " - unicode 1\n"
        " - unicode 2\n"
        " - ASCII 1\n"
        " - ASCII 2\n"
        " - ASCII 3\n"
        " - unicode right top\n"
        " - unicode right center\n"
        " - unicode right bottom"
      }
  };
}

std::vector<Translator::Example> Tree::Examples() {
  return {
      {"1 - Simple",
       "Linux\n"
       "  Android\n"
       "  Debian\n"
       "    Ubuntu\n"
       "      Lubuntu\n"
       "      Kubuntu\n"
       "      Xubuntu\n"
       "      Xubuntu\n"
       "    Mint\n"
       "  Centos\n"
       "  Fedora"},
  };
}
