# Atspi with GJS and Typescript

You can use write an atspi project using typescript by using vite and babel. This will transpile the typescript code into javascript which can be ran with gjs. Note that most node packages will not run in this environment.

Typescript types for gjs come from [ts-for-gir](https://github.com/gjsify/ts-for-gir). These can be npm installed since they do not depend on node.

## How to Run the Example

[`dump-tree.ts`](dump-tree.ts) dumps the accessibility hiearchy tree for a given application.

Install the packages and run the compiled output from the command line with gjs. Replace `"Firefox"` with the app name of your choice.

```
npm i
npm run build
gjs -m dist/main.js "Firefox"
```

The `-m` flag with gjs is used to allow ES modules.

You can also run `npm run start "Firefox"`. Note that if your npm version is packaged as a snap, you may have accessibility permission issues.
