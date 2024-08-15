// swift-tools-version: 5.8

let package = Package(
    name: "OneDS",
    platforms: [
        .iOS(.v10),
    ],
    products: [
        .library(
            name: "OneDS",
            type: .dynamic,
            targets: ["OneDS"]),
    ],
    dependencies: [
    ],
    targets: [
        .target(
            name: "OneDS",
            dependencies: [],
            path: "./lib",
            publicHeadersPath: "/lib/include/public",
            cSettings: [
            ],
            linkerSettings: [
                .linkedFramework("Network"),
                .linkedFramework("SystemConfiguration"),
                .linkedFramework("UIKit"),
                .linkedLibrary("stdc++"),
                .linkedLibrary("z"),
                .linkedLibrary("sqlite3")
            ])
    ]
)