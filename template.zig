// NOTE: 
// It turns out zig truly is to C what rust is to c++, premature optimization, excessive strictness for the sake of safety, and high effort to learn.
// This definitely has a good place in the world, but not what I'm looking for, thank you! (at least not for string processing, 
// without a library)

const std = @import("std");

// to run:
// zig run template.zig

// ---------------
// constants:


// ---------------
// functions:

pub fn main() !void {
    const stdout = std.io.getStdOut().writer();

    var arena = std.heap.ArenaAllocator.init(std.heap.page_allocator);
    defer arena.deinit();

    var list = std.ArrayList([]u8).init(arena.allocator());
    //list.ensureTotalCapacity(100) catch {
    //    try stdout.print("failed to allocate memory for image data\n", .{});
    //};

    // read lines from file
    {
        var infile = try std.fs.cwd().openFile("input", .{});
        defer infile.close();

        var buf_reader = std.io.bufferedReader(infile.reader());
        var in_stream = buf_reader.reader();

        var buf: [2048]u8 = undefined;
        while (try in_stream.readUntilDelimiterOrEof(&buf, '\n')) |line| {
            // do something with line...
            var splits = std.mem.split(u8, line, " ");
            while (splits.next()) |segment| {
                // NOTE: this nonsensical error must be a compiler bug
                var segment_copy: []u8 = try std.heap.page_allocator.alloc(u8, segment.len); 
                std.mem.copy(u8, segment_copy, segment);
                list.append(segment_copy) catch {
                    // do nothing
                };
            }
        }
    }

    var sum: i32 = 0;

    for (list) |line| {
        sum += std.fmt.parseInt(i32, line, 10);
    }

    try stdout.print("total: {}\n", .{sum});
    
    // free memory
    for (list) |line| {
        std.heap.page_allocator.free(line);
    }
}