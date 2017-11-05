#ifndef TRADER_H
#define TRADER_H

void Trader_tick();

int Trader_getClosestWarehouseForTradeCaravan(int figureId, int x, int y, int cityId, int distanceFromEntry, int *warehouseX, int *warehouseY);
int Trader_getClosestWarehouseForImportDocker(int x, int y, int cityId, int distanceFromEntry, int roadNetworkId, int *warehouseX, int *warehouseY);
int Trader_getClosestWarehouseForExportDocker(int x, int y, int cityId, int distanceFromEntry, int roadNetworkId, int *warehouseX, int *warehouseY);

int Trader_tryImportResource(int buildingId, int resourceId, int cityId);
int Trader_tryExportResource(int buildingId, int resourceId, int cityId);

#endif
