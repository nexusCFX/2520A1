//
//  MazeSolver.swift
//
//  Created by Brandon Chester on 2017-02-07.
//  Copyright © 2017 BrandySoft. All rights reserved.
//

import Foundation

class Position {
    let previous:Position?
    let x:Int
    let y:Int
    
    init(_ x: Int, _ y:Int, _ p:Position?) {
        self.previous = p
        self.x = x
        self.y = y
    }
}

struct Stack<Element> {
    var items = [Element]()
    
    mutating func push(_ item: Element) {
        items.append(item)
    }
    mutating func pop() -> Element {
        return items.removeLast()
    }
}

func solveMaze(_ grid:[[Character]]) {
    var maze = grid
    var curr = Position(0, 0, nil)
    
    for row in 0..<maze.count {
        for column in 0..<maze[row].count {
            if maze[row][column] == "o" {
                curr = Position(row, column, nil)
            }
        }
    }
    
    // Do DFS using Stack
    var stack = Stack<Position>()
    stack.push(curr)
    while stack.items.count != 0 {
        curr = stack.pop()
        print("X: \(curr.x). y:\(curr.y)")
        if maze[curr.x][curr.y] == "e" {
            stack.items.removeAll()
            
            // Go back through the previous positions
            var solvedMaze = grid
            var temp:Position! = curr.previous
            while (temp.previous != nil) {
                solvedMaze[temp.x][temp.y] = "@"
                temp = temp.previous
            }
            
            for row in 0..<solvedMaze.count {
                print(String(solvedMaze[row]))
            }
        } else if maze[curr.x][curr.y] != "*" {
            maze[curr.x][curr.y] = "*"
            stack.push(Position(curr.x, curr.y + 1, curr))
            stack.push(Position(curr.x, curr.y - 1, curr))
            stack.push(Position(curr.x - 1, curr.y, curr))
            stack.push(Position(curr.x + 1, curr.y, curr))
        }
    }
}

func main() {
    let path = "PATH_TO_FILE"
    do {
        let data = try String(contentsOfFile: path, encoding: .utf8)
        let myStrings = data.components(separatedBy: .newlines)
        var maze = [[Character]]()
        for string in myStrings {
            maze.append(Array(string.characters))
        }
        solveMaze(maze)
    } catch {
        print(error)
    }
}
main()
