# Flux Browser JS API Docs

## Tab Management

```ts
window.tabs.requestSplitTab(url: string): null
window.tabs.requestSplitTab(): null
window.tabs.requestFlipTabs(): null
window.tabs.requestNewGroup(url: string): null
window.tabs.addTabs(tabs: Array<Array<string>>): boolean
```
## File API

```ts
window.fs.listDir(path: string, subdir?: boolean) : Array<{path: string, isDir: boolean, dirs: Array<{path: string, isDir: boolean}>}>
window.fs.getFileUrl(): string
window.fs.getFolderUrl(): string
```
## Dialog API
```ts
window.dialog.closeDialog()
```